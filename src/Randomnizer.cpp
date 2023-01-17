#include <array>

#include "Save.h"
#include "Randomnizer.h"
#include "Random.h"

namespace KysChess {

namespace {


static std::unordered_set<Role*>& getRejected()
{
    // ÿ�����ʱ �ϴ�û�б�����Ĳ����ٳ���
    static std::unordered_set<Role*> rejected;
    return rejected;
}

void removeFromRejected(Role* r)
{
    auto& rejected = getRejected();
    rejected.erase(r);
}

using ProbArray = std::array<int, 5>;

constexpr std::array<std::array<int, 5>, 10> weights = {
    ProbArray{100, 0, 0, 0, 0},
    ProbArray{70, 30, 0, 0, 0},
    ProbArray{60, 35, 5, 0, 0},
    ProbArray{50, 35, 15, 0, 0},
    ProbArray{40, 35, 23, 2, 0},
    ProbArray{33, 30, 30, 7, 0},
    ProbArray{30, 30, 30, 10, 0},
    ProbArray{24, 30, 30, 15, 1},
    ProbArray{22, 30, 25, 20, 3},
    ProbArray{19, 25, 25, 25, 6}};


// TODO: �ĳɴ�r���ݶ�ȡ?
const std::array<std::vector<int>, 5> chessIdxOfPrice = {
    std::vector<int>{
        130, // �����
        131, // ���
        132, // ������
        133, // ��ϣ��
        134, // �Ű���
        135, // ȫ��
        136, // ��СӨ
        63,  // ��Ӣ
        84,  // ����
        160, // �����
        161, // ��Ī��
        45,  // ѦĽ��
        47,  // ����
        104, // ����
        105  // ����
    },
    std::vector<int>{
        54,  // Ԭ��־
        37,  // ����
        97,  // Ѫ������
        56,  // ����
        44,  // ������
        48,  // ��̹֮
        99,  // Ҷ����
        100, // ���к�
        102, // ����
        115  // ���Ǻ�
    },
    std::vector<int>{
        55,  // ����
        67,  // ��ǧ��
        68,  // �𴦻�
        59,  // С��Ů
        46,  // ������
        51,  // Ľ�ݸ�
        53,  // ����
        70,  // ����
        98,  // ������
        103, // �Ħ��
        112, // ��Զɽ
        113  // Ľ�ݲ�
    },
    std::vector<int>{
        57,  // ��ҩʦ
        60,  // ŷ����
        64,  // �ܲ�ͨ
        65,  // һ��
        69,  // ���߹�
        58,  // ���
        62,  // ���ַ���
        49,  // ����
        50,  // �Ƿ�
        117, // ��ɽͯ��
        118  // ����ˮ
    },
    std::vector<int>{
        129, // ������
        592, // �������
        114, // ɨ����ɮ
        116  // ������
    },
};

} // namespace


Role* selectFromPool(int price)
{
    static Random<> rand;

    for (;;) {
        auto idx = chessIdxOfPrice[price][rand.rand_int(chessIdxOfPrice[price].size())];
        auto role = Save::getInstance()->getRole(idx);
        auto& rejected = getRejected();
        if (price <= 3 && rejected.contains(role)) {
            continue;
        }
        return role;
    }
}

std::vector<Role*> getChess(int level, int pieces)
{
    std::vector<Role*> roles;
    static Random<> rand;
    
    for (int i = 0; i < pieces; ++i) {
        // Ӧ���� 0~99
        auto val = rand.rand_int(100);
        auto cur = 0;
        for (int price = 0; price < weights[level].size(); ++price)
        {
            auto w = weights[level][price];
            cur += w;
            if (val < cur)
            {
                roles.push_back(selectFromPool(price));
            }
        }
    }

    auto& rejected = getRejected();
    rejected.clear();
    for (auto r : roles) {
        rejected.insert(r); 
    }

    return roles;
}

} // namespace KysChess