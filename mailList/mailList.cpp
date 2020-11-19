/**
* 作者：刘时明
* 时间：2020/11/15 0015
*/
#include "mailList.h"
#include "list.h"
#include <uuid/uuid.h>
#include <istream>
#include <fstream>
#include <cstdio>

static LinkedList<DATA *> *list = new LinkedList<DATA *>;

/**
 * 加载
 */
void load()
{
    std::ifstream input;
    input.open(STR2(FILE_PATH), std::ios::binary);
    if (!input.is_open())
    {
        std::cout << "文件不存在" << std::endl;
        return;
    }
    while (!input.eof())
    {
        DATA *data = new DATA;
        input.read(reinterpret_cast<char *>(data), sizeof(DATA));
        if (input.fail())
        {
            break;
        }
        list->add(data);
    }
    input.close();
}

/**
 * 缓存
 */
void save()
{
    std::fstream output(STR2(FILE_PATH), std::ios::out | std::ios::binary);
    if (!output.is_open())
    {
        std::cout << "文件不可打开" << std::endl;
        return;
    }
    for (int i = 0; i < list->size(); ++i)
    {
        output.write(reinterpret_cast<char *>(list->get(i)), sizeof(DATA));
    }
    output.close();
}

/**
 * 获取一个操作类型
 * @return
 */
int get_type()
{
    std::cout << "-----请输入操作类型-----" << std::endl;
    std::cout << "0.退出" << std::endl;
    std::cout << "1.新增" << std::endl;
    std::cout << "2.修改" << std::endl;
    std::cout << "3.删除" << std::endl;
    std::cout << "4.列表" << std::endl;
    std::cout << "5.搜索" << std::endl;
    int type = 0;
    std::cin >> type;
    return type;
}

/**
 *
 */
void loop_handler()
{
    while (true)
    {
        int type = get_type();
        switch (type)
        {
            case 0:
                std::cout << "exit" << std::endl;
                return;
            case 1:
                add_list();
                break;
            case 2:
                std::cout << "修改" << std::endl;
                break;
            case 3:
                delete_list();
                break;
            case 4:
                show_list();
                break;
            case 5:
                search();
                break;
            default:
                std::cout << "不能识别的命令" << std::endl;
                break;
        }
        std::cout << "-----一次会话结束-----" << std::endl;
    }
}

void add_list()
{
    auto data = new DATA;
    std::string uuid = generate_uuid();
    strcpy(data->id, uuid.c_str());
    std::cout << "输入名字:" << std::endl;
    std::cin >> data->name;
    std::cout << "输入电话:" << std::endl;
    std::cin >> data->phone;
    list->add(data);
    save();
}

void show_list()
{
    if (list->size() == 0)
    {
        std::cout << "列表空空如也!" << std::endl;
        return;
    }
    for (int i = 0; i < list->size(); i++)
    {
        auto temp = list->get(i);
        std::cout << "phone=" << temp->phone << "," << "name=" << temp->name << std::endl;
    }
}

void delete_list()
{
    std::cout << "输入名字或者电话号码:" << std::endl;
    char target[64];
    std::cin >> target;
    for (int i = 0; i < list->size(); ++i)
    {
        auto temp = list->get(i);
        if (strcmp(temp->name, target) == 0 || strcmp(temp->phone, target) == 0)
        {
            list->remove(i);
            save();
            std::cout << "删除完成!" << std::endl;
            return;
        }
    }
    std::cout << "找不到目标元素:" << std::endl;
}

void search()
{
    std::cout << "输入名字或者电话号码:" << std::endl;
    char target[64];
    std::cin >> target;
    for (int i = 0; i < list->size(); ++i)
    {
        auto temp = list->get(i);
        if (strcmp(temp->name, target) == 0 || strcmp(temp->phone, target) == 0)
        {
            std::cout << "phone=" << temp->phone << "," << "name=" << temp->name << std::endl;
            return;
        }
    }
    std::cout << "找不到目标元素:" << std::endl;
}

/**
 * uuid
 * @return
 */
std::string generate_uuid()
{
    char buf[GUID_LEN] = {0};
    uuid_t uu;
    uuid_generate(uu);
    int32_t index = 0;
    for (int32_t i = 0; i < 16; i++)
    {
        int32_t len = i < 15 ? sprintf(buf + index, "%02X-", uu[i]) : sprintf(buf + index, "%02X", uu[i]);
        if (len < 0)
        {
            return std::move(std::string(""));
        }
        index += len;
    }
    return std::move(std::string(buf));
}
