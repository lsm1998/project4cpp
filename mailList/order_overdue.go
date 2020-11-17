/*
* 作者：刘时明
* 时间：2020/10/28 0028-16:16
* 作用：账单逾期费用定时job
 */
package jobs

import (
	"fmt"
	"git.funxdata.com/funxcloud/funxcloud/cmd/internal/util"
	"git.funxdata.com/funxcloud/funxcloud/pkgs/as/models/orderv3"
	"git.funxdata.com/funxcloud/funxcloud/types"
	"github.com/jinzhu/gorm"
	"github.com/sirupsen/logrus"
	"github.com/spf13/cobra"
	"time"
)

const LateFee = 5.0 / 1000

func overdueFeeUpdateCmd() *cobra.Command {
	cmd := &cobra.Command{
		Use:   "overdue-fee-update",
		Short: "账单逾期费用订单更新",
		Run: func(cmd *cobra.Command, args []string) {
			logrus.Infof("overdue-fee-update.")
			db := util.GetDB()
			log := logrus.WithField("cmd", "overdue-fee-update")

			size := 100
			page := 0

			overdueOrderMap := make(map[string][]*types.Order)
			for {
				// 每次处理100条
				var list []*types.Order
				// 获取所有已经逾期的订单
				if err := db.Model((*types.Order)(nil)).Select("*").
					Where("late_fee_start_at < NOW() and status=?", types.OrderStatusPending).
					Offset(page * size).Limit(size).Find(&list).Error; err != nil {
					return
				}
				page++
				for _, v := range list {
					key := fmt.Sprintf(`%d-%d`, v.ResidentID, v.RoomID)
					list, ok := overdueOrderMap[key]
					if ok {
						list = append(list, v)
						overdueOrderMap[key] = list
					} else {
						overdueOrderMap[key] = []*types.Order{v}
					}
				}
				if len(list) < size {
					break
				}
			}
			now := time.Now()
			for _, list := range overdueOrderMap {
				var temp types.Order
				var minLateFeeStartAt = time.Now()
				for _, v := range list {
					// 逾期天数
					days := int(now.Sub(v.LateFeeStartAt.Local()).Hours() / 24)
					// 计算逾期费用
					temp.Money = temp.Money.Add(v.Money.Mul(float64(days) * LateFee))
					temp.ResidentID = v.ResidentID
					temp.StoreID = v.StoreID
					// 如果多笔账单产生滞纳金，则滞纳金开始计算时间取其中最小的
					if minLateFeeStartAt.Unix() > v.LateFeeStartAt.Local().Unix() {
						temp.BeginTime = v.LateFeeStartAt.Local()
						minLateFeeStartAt = temp.BeginTime
						temp.LateFeeStartAt = v.LateFeeStartAt
						temp.RoomID = v.RoomID
					}
				}
				temp.ShouldPay = temp.Money
				temp.CreatedAt = now
				temp.UpdatedAt = now
				temp.Year = temp.LateFeeStartAt.Year()
				temp.Month = temp.LateFeeStartAt.Month()
				temp.EndTime = now
				temp.MergeTime = fmt.Sprintf(`%d-%d`, temp.Year, temp.Month)
				temp.Number = orderv3.GenerateOrderNumber()
				temp.PayCircle = temp.MergeTime
				temp.Status = types.OrderStatusPending
				temp.Remark = fmt.Sprintf(`%s至%s的滞纳金`, temp.BeginTime.Format("2006-01-02"), temp.EndTime.Format("2006-01-02"))
				temp.Type = types.OrderTypeOverdue
				temp.GroupType = types.OrderGroupTypeService

				if temp.StoreID != 1 {
					continue
				}
				if err := lateFeeSaveOrUpdate(db, &temp); err != nil {
					log.Errorf("lateFeeSaveOrUpdate error! err=%v", err)
				}
			}
		},
	}
	return cmd
}

func lateFeeSaveOrUpdate(db *gorm.DB, order *types.Order) error {
	if order == nil {
		return nil
	}
	if err := db.Model((*types.Order)(nil)).
		Select("id").
		Where("resident_id=?", order.ResidentID).
		Where("room_id=?", order.RoomID).
		Where("type=?", types.OrderTypeOverdue).
		First(&order).Error; err != nil {
		if err := db.Save(order).Error; err != nil {
			return err
		}
	} else {
		if err := db.Table(order.TableName()).Where("id=?", order.ID).Updates(map[string]interface{}{
			"money":      order.Money,
			"updated_at": time.Now(),
			"year":       order.Year,
			"should_pay": order.ShouldPay,
			"month":      order.Month,
			"merge_time": order.MergeTime,
			"pay_circle": order.PayCircle,
			"begin_time": order.BeginTime,
			"end_time":   order.EndTime,
			"remark":     order.Remark,
		}).Error; err != nil {
			return err
		}
	}
	return nil
}
