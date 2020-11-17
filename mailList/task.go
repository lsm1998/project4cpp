package orderupdaterecord

import (
	"errors"
	"time"

	"git.funxdata.com/brisk/brisk/gerr"
	"git.funxdata.com/brisk/commons/random"
	"git.funxdata.com/funxcloud/funxcloud/core/global"
	"git.funxdata.com/funxcloud/funxcloud/core/sysconfig"
	"git.funxdata.com/funxcloud/funxcloud/pkgs/accounts/models/employee"
	"git.funxdata.com/funxcloud/funxcloud/pkgs/as/models/order"
	"git.funxdata.com/funxcloud/funxcloud/pkgs/eam/models/meters"
	"git.funxdata.com/funxcloud/funxcloud/pkgs/eam/models/room"
	"git.funxdata.com/funxcloud/funxcloud/pkgs/eam/models/store"
	"git.funxdata.com/funxcloud/funxcloud/pkgs/oa/models/taskflow"
	"git.funxdata.com/funxcloud/funxcloud/pkgs/pms/models/resident"
	"git.funxdata.com/funxcloud/funxcloud/types"
	"github.com/funxdata/wechat/message"
	"github.com/jinzhu/gorm"
	"github.com/sirupsen/logrus"
)

// OrderUpdateWrapper ...
type OrderUpdateWrapper struct {
	db         *gorm.DB
	logger     *logrus.Entry
	record     *BatchModifyRequest
	store      *types.Store
	room       *types.Room
	resident   *types.Resident
	taskflowID uint
}

func init() {
	taskflow.Registry(string(types.TaskflowTypeOrderUpdate), LoadOrderUpdateWrapper)
}

// LoadOrderUpdateWrapper 加载Wrapper
func LoadOrderUpdateWrapper(db *gorm.DB, log *logrus.Entry, task *types.Taskflow, empid uint) (taskflow.Tasker, error) {
	out := &OrderUpdateWrapper{
		db:         db,
		logger:     log,
		taskflowID: task.ID,
	}
	// store
	st, err := store.Get(db, task.StoreID)
	if err != nil {
		return nil, err
	}
	out.store = st

	// room
	ro, err := room.Get(db, task.RoomID)
	if err != nil {
		return nil, err
	}
	out.room = ro

	// detail
	detail, err := GetByTaskflowID(db, log, task.ID)
	if err != nil {
		return nil, err
	}
	// 构造record
	rec, err := SetRecord(detail)
	if err != nil {
		return nil, err
	}
	out.record = rec

	// resident
	res, err := resident.GetResidentByID(db, rec.ResidentID)
	if err != nil {
		return nil, err
	}
	out.resident = res

	return out, nil
}

// BatchModifyRequest 批量修改账单请求参数
type BatchModifyRequest struct {
	Number      string
	StoreID     uint                       `json:"store_id"`
	ResidentID  uint                       `json:"resident_id"`
	RoomID      uint                       `json:"room_id"`
	EmployeeID  uint                       `json:"employee_id"`
	PayCircle   string                     `json:"pay_circle"`
	Adds        []*types.OrderUpdateRecoed `json:"adds"`
	Modifies    []*types.OrderUpdateRecoed `json:"modifies"`
	Deletes     []*types.OrderUpdateRecoed `json:"deletes"`
	AddReadings []*meters.Reading          `json:"add_readings"`
}

var _ taskflow.Tasker = (*OrderUpdateWrapper)(nil)

// Create 创建账单修改任务流
func Create(db *gorm.DB, logger *logrus.Entry, opt *BatchModifyRequest) (*BatchModifyRequest, error) {
	if err := CheckOrderUpdateAudit(db, logger, opt); err != nil {
		logger.Warnf("CheckOrderUpdateAudit failed %s", err)
		return nil, err
	}
	r, err := room.Get(db, opt.RoomID)
	if err != nil {
		logger.Warnf("Get room failed %s", err)
		return nil, err
	}
	st, err := store.Get(db, r.StoreID)
	if err != nil {
		logger.Warnf("Get store failed %s", err)
		return nil, err
	}
	re, err := resident.Get(db, opt.ResidentID)
	if err != nil {
		logger.Warnf("Get store failed %s", err)
		return nil, err
	}
	// 检查水电读数
	if len(opt.AddReadings) > 0 {
		// 1. 检查住户是在住住户
		if r.ResidentID != re.ID {
			return nil, gerr.BadRequest("该住户不是在住住户")
		}
		if r.Status != types.RoomStatusRent || re.Status != types.ResidentStatusNormal {
			return nil, gerr.BadRequest("请检查住户状态")
		}
		// 2. 检查表类型不重复
		if len(opt.AddReadings) > 1 {
			mp := make(map[types.DeviceType]float64, len(opt.AddReadings))
			for _, reading := range opt.AddReadings {
				if _, ok := mp[reading.Type]; ok {
					return nil, gerr.BadRequest("水电读数添加账单类型重复")
				}
				mp[reading.Type] = reading.Reading
			}
		}
		// 3. 检查读数
		for _, reading := range opt.AddReadings {
			err := meters.CheckReading(db, opt.RoomID, reading)
			if err != nil {
				return nil, err
			}
		}
	}

	// 开启事务
	tx := db.Begin()
	ret, err := CreateOrderUpdateRecord(tx, logger, re, opt)
	if err != nil {
		tx.Rollback()
		logger.Warnf("CreateOrderUpdateRecord failed %s", err)
		return nil, err
	}
	// 创建费用不用走审批，修改和关闭才走审批
	if len(ret.Deletes) > 0 || len(ret.Modifies) > 0 {
		pw := &OrderUpdateWrapper{
			db:       tx,
			logger:   logger,
			room:     r,
			store:    st,
			record:   ret,
			resident: re,
		}
		topt := &taskflow.CreateOption{
			Type:               types.TaskflowTypeOrderUpdate,
			StoreID:            r.StoreID,
			RoomTypeID:         r.RoomTypeID,
			RoomID:             r.ID,
			TaskflowCreateRole: types.OperatorRoleEmployee,
			EmployeeID:         opt.EmployeeID,
			TaskGroup:          types.TaskGroupOrderUpdate,
		}

		t, err := taskflow.Create(tx, topt, pw)
		if err != nil {
			tx.Rollback()
			logger.Warnf("Create Task failed %s", err)
			return nil, err
		}

		err = tx.Table(global.TOrderUpdateRecord).
			Where("number = ?", opt.Number).
			Where("action in (?)", []types.ActionType{types.ActionTypeClosed, types.ActionTypeUpdate}).
			Update("taskflow_id", t.ID).
			Error
		if err != nil {
			tx.Rollback()
			logger.Warnf("update record taskflow_id failed %s", err)
			return nil, err
		}
	}
	if err := tx.Commit().Error; err != nil {
		return nil, err
	}
	return opt, nil
}

// OrderUpdateDetail 详情
type OrderUpdateDetail struct {
	*BatchModifyRequest `json:",inline"`
	StoreName           string `json:"store_name"`
	RoomNumber          string `json:"room_number"`
	ResidentName        string `json:"resident_name"`
}

// TaskShowDetail 任务流展示
func (p *OrderUpdateWrapper) TaskShowDetail() (interface{}, error) {
	out, err := GetByTaskflowID(p.db, p.logger, p.taskflowID)
	if err != nil {
		return nil, err
	}
	// out := &OrderUpdateDetail{
	// 	BatchModifyRequest: p.record,
	// 	StoreName:          p.store.Name,
	// 	RoomNumber:         p.room.Number,
	// 	ResidentName:       p.resident.Name,
	// }
	return out, nil
}

// TaskComplete 任务流完成
func (p *OrderUpdateWrapper) TaskComplete(db *gorm.DB) (interface{}, error) {
	bat, err := GetByNumber(db, p.logger, p.record.Number)
	if err != nil {
		return nil, err
	}
	for _, val := range bat {
		val.Status = types.OrderUpdateStatusEffect
		val.EffectTime = time.Now()
		_, err := UpdateByID(db, p.logger, val, val.ID)
		if err != nil {
			return nil, err
		}
		switch val.Action {
		case types.ActionTypeCreate:
			// 修改为直接创建费用，不经过审批
		case types.ActionTypeUpdate:
			o := &order.UpdateOrderOption{
				Money: val.Money,
			}
			_, err := order.UpdateOrderByID(db, val.OrderID, o)
			if err != nil {
				return nil, err
			}
			s := val.OrderStatus
			_, err = order.UpdateOrderStatusByID(db, val.OrderID, s)
			if err != nil {
				return nil, err
			}
		case types.ActionTypeClosed:
			_, err = order.CloseOrderByID(db, val.OrderID)
			if err != nil {
				return nil, err
			}
		}
	}
	return p, nil
}

// generateOrderWithUpdateRecoed 通过账单更新记录创建费用
func generateOrderWithUpdateRecoed(db *gorm.DB, val *types.OrderUpdateRecoed) error {
	o := &types.Order{
		SimpleFee: types.SimpleFee{
			StoreID:      val.StoreID,
			RoomID:       val.RoomID,
			ResidentID:   val.ResidentID,
			Type:         val.OrderType,
			Money:        val.Money,
			Remark:       val.Remark,
			PayCircle:    val.PayCircle,
			BeginTime:    val.BeginTime,
			EndTime:      val.EndTime,
			BeginReading: val.BeginReading,
			EndReading:   val.EndReading,
		},
		GroupType:  val.OrderType.ToGroupType(),
		EmployeeID: val.EmployeeID,
		Number:     val.Number,
		ShouldPay:  val.Money,
		Status:     types.OrderStatusPending,
		Year:       val.EndTime.Year(),
		Month:      val.EndTime.Month(),
		AddWay:     types.OrderAddWayAdd,
	}
	// ------ 手动新增账单增加逾期日期（2020-11-6） ------
	now := time.Now()
	if now.Day() <= 5 {
		lateFeeStartAt := time.Date(now.Year(), now.Month(), 6, 0, 0, 0, 0, time.Local)
		o.LateFeeStartAt = &lateFeeStartAt
	} else if now.Day() >= 25 {
		lateFeeStartAt := time.Date(now.Year(), now.Month()+1, 6, 0, 0, 0, 0, time.Local)
		o.LateFeeStartAt = &lateFeeStartAt
	}
	// ------ 手动新增账单增加逾期日期（2020-11-6） ------
	_, err := order.CreateOrder(db, o)
	if err != nil {
		return err
	}
	return nil
}

// TaskClose 任务流关闭
func (p *OrderUpdateWrapper) TaskClose(db *gorm.DB) (interface{}, error) {
	err := db.Table(global.TOrderUpdateRecord).
		Where("taskflow_id = ?", p.taskflowID).
		Update("status", types.OrderUpdateStatusInvalid).
		Error
	if err != nil {
		return nil, err
	}

	odrIDs := []uint{}
	db.Table(global.TOrderUpdateRecord).
		Where("deleted_at IS NULL").
		Where("taskflow_id = ?", p.taskflowID).
		Pluck("order_id", &odrIDs)
	err = db.Table(global.TOrder).
		Where("id in (?)", odrIDs).
		Update("boss_order.status", types.OrderStatusPending).
		Error
	if err != nil {
		return nil, err
	}

	return p, nil
}

// TaskNotifyData 任务流通知信息
func (p *OrderUpdateWrapper) TaskNotifyData(*types.Taskflow) (map[string]*message.DataItem, error) {
	// tp := p.price.Type.ToString()
	e, err := employee.Get(p.db, p.record.EmployeeID)
	if err != nil {
		return nil, err
	}
	data := map[string]*message.DataItem{
		"first":    &message.DataItem{Value: "有新的账单修改审核"},
		"keyword1": &message.DataItem{Value: p.store.Name + "-" + p.room.Number + "-" + p.resident.Name + "账单修改申请"},
		"keyword2": &message.DataItem{Value: e.Name},
		"keyword3": &message.DataItem{Value: time.Now().Format("2006-01-02 15:04:05")},
		"keyword4": &message.DataItem{Value: ""},
		"remark":   &message.DataItem{Value: "请尽快处理审批!"},
	}
	return data, nil
}

// TaskNotifyMiniPath 通知消息跳转地址
func (c *OrderUpdateWrapper) TaskNotifyMiniPath() string {
	return ""
}

// TaskNotifyTpl 任务流通知模板
func (p *OrderUpdateWrapper) TaskNotifyTpl() (string, error) {
	return sysconfig.Get().WechatEmployeeNotifyTpl.TaskRemind, nil
}

// TaskRejectTo 驳回
func (p *OrderUpdateWrapper) TaskRejectTo() taskflow.RejectTo {
	return taskflow.RejectToClose
}

// CheckOrderUpdateAudit 校验本次账单修改审核是否已有账单在审核中
func CheckOrderUpdateAudit(db *gorm.DB, log *logrus.Entry, opt *BatchModifyRequest) error {
	records, err := GetByPayCirle(db, log, opt.ResidentID, opt.PayCircle)
	if err != nil && err != gorm.ErrRecordNotFound {
		return err
	}

	for _, item := range opt.Adds {
		if item.OrderType == "" {
			return errors.New("请选择账单类型")
		}
	}
	var allRecords []*types.OrderUpdateRecoed
	allRecords = append(allRecords, opt.Adds...)
	allRecords = append(allRecords, opt.Modifies...)
	for _, item := range allRecords {
		for _, record := range records {
			if record.Status == types.OrderUpdateStatusAudit && record.ID == item.ID {
				return gerr.BadRequest("此账单已存在修改审批")
			}
		}
	}

	return nil
}

// CreateOrderUpdateRecord 批量创建修改记录
func CreateOrderUpdateRecord(db *gorm.DB, log *logrus.Entry, rsdt *types.Resident, opt *BatchModifyRequest) (*BatchModifyRequest, error) {
	now := time.Now()
	number := now.Format("20060102150405") + random.RandomStringNumber(4)
	opt.Number = number
	if len(opt.AddReadings) > 0 {
		// 创建账单修改记录
		for _, val := range opt.AddReadings {
			// 未生效的水电读数失效
			err := meters.InvalidRoomWaitReading(db, opt.RoomID, val.Type)
			if err != nil {
				return nil, err
			}

			orderType, err := val.Type.ParseOrderType()
			if err != nil {
				return nil, err
			}
			lastRecord, err := meters.GetLastReading(db, opt.RoomID, val.Type)
			if err != nil {
				return nil, err
			}
			fee := &meters.FeeOption{
				Mtype:          val.Type,
				LastReading:    lastRecord.ThisReading,
				CurrentReading: val.Reading,
				RoomID:         opt.RoomID,
			}
			money, err := meters.ReadingFee(db, fee)

			if money.IsZero() {
				continue
			}
			in := &types.OrderUpdateRecoed{
				Number:       number,
				StoreID:      opt.StoreID,
				ResidentID:   opt.ResidentID,
				RoomID:       opt.RoomID,
				EmployeeID:   opt.EmployeeID,
				OrderType:    orderType,
				OriginMoney:  0,
				Money:        money,
				PayCircle:    opt.PayCircle,
				BeginTime:    lastRecord.ThisTime,
				EndTime:      val.ThisTime,
				BeginReading: lastRecord.ThisReading,
				EndReading:   val.Reading,
				Remark:       val.Remark,
				LaunchTime:   now,
				EffectTime:   now,
				Status:       types.OrderUpdateStatusEffect,
				Action:       types.ActionTypeCreate,
				OrderStatus:  types.OrderStatusPending,
			}
			// 创建
			err = Add(db, log, in)
			if err != nil {
				return nil, err
			}
			// 创建账单
			if err := generateOrderWithUpdateRecoed(db, in); err != nil {
				return nil, err
			}
		}
		// 创建水电读数
		readingTransfers, err := meters.GenerateNormalReading(db, rsdt, opt.AddReadings)
		if err != nil {
			return nil, err
		}
		for _, item := range readingTransfers {
			err := db.Create(item).Error
			if err != nil {
				return nil, err
			}
		}
	}

	if len(opt.Adds) > 0 {
		for _, val := range opt.Adds {
			in := &types.OrderUpdateRecoed{
				Number:      number,
				StoreID:     opt.StoreID,
				ResidentID:  opt.ResidentID,
				RoomID:      opt.RoomID,
				EmployeeID:  opt.EmployeeID,
				OrderID:     val.OrderID,
				OrderType:   val.OrderType,
				OriginMoney: 0,
				Money:       val.Money,
				PayCircle:   opt.PayCircle,
				BeginTime:   val.BeginTime,
				EndTime:     val.EndTime,
				Remark:      val.Remark,
				LaunchTime:  now,
				Status:      types.OrderUpdateStatusEffect,
				Action:      types.ActionTypeCreate,
				OrderStatus: types.OrderStatusPending,
				EffectTime:  now,
			}
			err := Add(db, log, in)
			if err != nil {
				return nil, err
			}
			// 修改为直接创建费用，不经过审批
			if err := generateOrderWithUpdateRecoed(db, in); err != nil {
				return nil, err
			}
		}
	}
	if len(opt.Modifies) > 0 {
		out := []*types.OrderUpdateRecoed{}
		for _, val := range opt.Modifies {
			o, err := order.GetOrderByID(db, val.OrderID)
			if err != nil {
				return nil, err
			}
			in := &types.OrderUpdateRecoed{
				Number:      number,
				StoreID:     opt.StoreID,
				ResidentID:  opt.ResidentID,
				RoomID:      opt.RoomID,
				EmployeeID:  opt.EmployeeID,
				OrderID:     o.ID,
				OrderType:   o.Type,
				OriginMoney: val.OriginMoney,
				Money:       val.Money,
				PayCircle:   opt.PayCircle,
				BeginTime:   o.BeginTime,
				EndTime:     o.EndTime,
				Remark:      val.Remark,
				LaunchTime:  now,
				Status:      types.OrderUpdateStatusAudit,
				Action:      types.ActionTypeUpdate,
				OrderStatus: o.Status,
			}

			if err := Add(db, log, in); err != nil {
				log.Warningf("Create Audit 新增账单修改记录失败，%s", err)
				return nil, err
			}
			s := types.OrderStatusAuditing
			_, err = order.UpdateOrderStatusByID(db, o.ID, s)
			if err != nil {
				log.Warningf("Create Audit 账单状态更新失败，%s", err)
				return nil, err
			}
			out = append(out, in)
		}
		opt.Modifies = out
	}
	if len(opt.Deletes) > 0 {
		out := []*types.OrderUpdateRecoed{}
		for _, val := range opt.Deletes {
			o, err := order.GetOrderByID(db, val.OrderID)
			if err != nil {
				return nil, err
			}
			in := &types.OrderUpdateRecoed{
				Number:      number,
				StoreID:     opt.StoreID,
				ResidentID:  opt.ResidentID,
				RoomID:      opt.RoomID,
				EmployeeID:  opt.EmployeeID,
				OrderID:     o.ID,
				OrderType:   o.Type,
				OriginMoney: val.OriginMoney,
				Money:       0,
				PayCircle:   opt.PayCircle,
				BeginTime:   o.BeginTime,
				EndTime:     o.EndTime,
				Remark:      val.Remark,
				LaunchTime:  now,
				Status:      types.OrderUpdateStatusAudit,
				Action:      types.ActionTypeClosed,
			}

			if err := Add(db, log, in); err != nil {
				log.Warningf("Create Audit 新增账单修改记录失败，%s", err)
				return nil, err
			}
			s := types.OrderStatusAuditing
			_, err = order.UpdateOrderStatusByID(db, o.ID, s)
			if err != nil {
				log.Warningf("Create Audit 账单状态更新失败，%s", err)
				return nil, err
			}
			out = append(out, in)
		}
		opt.Deletes = out
	}
	return opt, nil
}

// SetRecord 构造BatchModifyRequest
func SetRecord(in []*Detail) (*BatchModifyRequest, error) {

	out := &BatchModifyRequest{
		Number:     in[0].Number,
		StoreID:    in[0].StoreID,
		ResidentID: in[0].ResidentID,
		RoomID:     in[0].RoomID,
		EmployeeID: in[0].EmployeeID,
		PayCircle:  in[0].PayCircle,
	}
	adds := []*types.OrderUpdateRecoed{}
	modifies := []*types.OrderUpdateRecoed{}
	deletes := []*types.OrderUpdateRecoed{}

	for _, val := range in {
		switch val.Action {
		case types.ActionTypeCreate:
			adds = append(adds, val.OrderUpdateRecoed)
		case types.ActionTypeUpdate:
			modifies = append(modifies, val.OrderUpdateRecoed)
		case types.ActionTypeClosed:
			deletes = append(deletes, val.OrderUpdateRecoed)
		}
	}
	return out, nil
}
