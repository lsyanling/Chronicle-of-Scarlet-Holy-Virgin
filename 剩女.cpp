
// 地图上0表示墙壁，1表示空地，2表示冰棱，3表示剩女
#include <set>
#include <tuple>
#include <vector>
#include <fstream>
#include <iostream>

using std::set;
using std::tuple;
using std::vector;
using std::ifstream;
using std::ofstream;
// using std::println;

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// 地图是反的，对于一张地图，最下面是第0行，最左边是第0列，所以UP是x+1，DOWN是x-1，LEFT是y-1，RIGHT是y+1
vector<vector<int>> Map{
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 2, 2, 1, 1, 1, 2, 2, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 2, 2, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 2, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 1, 1, 2, 1, 2, 2, 1, 1, 2, 1, 1, 1, 2, 1, 2, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 2, 2, 1, 1, 2, 1, 0, 0, 0},
    {0, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 2, 1, 1, 1, 0, 0},
    {0, 1, 2, 1, 1, 2, 1, 2, 2, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 0, 0},
    {0, 1, 1, 2, 1, 1, 2, 2, 1, 1, 3, 1, 1, 2, 1, 2, 1, 2, 1, 1, 0},
    {0, 0, 1, 2, 2, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 2, 2, 1, 1, 0},
    {0, 0, 1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 2, 1, 2, 1, 1, 1, 2, 1, 0},
    {0, 0, 1, 2, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 2, 1, 1, 2, 2, 1, 2, 2, 2, 1, 2, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 2, 1, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 2, 2, 1, 1, 2, 2, 1, 2, 2, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

// 先探路，把遇到的2的坐标存下来，如果可行就写Map，方便回溯
vector<vector<tuple<int, int>>> writeBuffer;
// 保存UP, DOWN, LEFT, RIGHT的序列，即题解步骤
vector<Direction> solution;
// state三元组，{num, x, y}
set<tuple<int, int, int>> states;
// 同上，states用于查找，statesStack用于回溯
vector<tuple<int, int, int>> statesStack;

// 连续20步没撞到2，剪枝
int countDrop = 0;

// 移动一步，返回移动后的state三元组，num是Map中剩余的2的数量，x, y是当前所在坐标
// 由于撞到3是终止条件，所以对2是返回自己的坐标，撞到3是直接返回3的坐标，在backtrack中有单独的判断
auto move(int num, int x, int y, Direction direction) {
    int stopFlag = 0;
    int countFlag = 1;
    writeBuffer.push_back({});
    switch (direction) {
    // x增大
    case Direction::UP:
        for (; stopFlag != 2; x++) {
            if (Map[x + 1][y] == 0) {
                break;
            }
            if (Map[x + 1][y] == 2) {
                stopFlag++;
                countFlag = 0;
                num--;
                writeBuffer.back().push_back({x + 1, y});
                if (stopFlag == 2) {
                    break;
                }
            }
            if (Map[x + 1][y] == 3) {
                return tuple{num, x + 1, y};
            }
        }
        break;
    // x减小
    case Direction::DOWN:
        for (; stopFlag != 2; x--) {
            if (Map[x - 1][y] == 0) {
                break;
            }
            if (Map[x - 1][y] == 2) {
                stopFlag++;
                countFlag = 0;
                num--;
                writeBuffer.back().push_back({x - 1, y});
                if (stopFlag == 2) {
                    break;
                }
            }
            if (Map[x - 1][y] == 3) {
                return tuple{num, x - 1, y};
            }
        }
        break;
    // y减小
    case Direction::LEFT:
        for (; stopFlag != 2; y--) {
            if (Map[x][y - 1] == 0) {
                break;
            }
            if (Map[x][y - 1] == 2) {
                stopFlag++;
                countFlag = 0;
                num--;
                writeBuffer.back().push_back({x, y - 1});
                if (stopFlag == 2) {
                    break;
                }
            }
            if (Map[x][y - 1] == 3) {
                return tuple{num, x, y - 1};
            }
        }
        break;
    // y增大
    case Direction::RIGHT:
        for (; stopFlag != 2; y++) {
            if (Map[x][y + 1] == 0) {
                break;
            }
            if (Map[x][y + 1] == 2) {
                stopFlag++;
                countFlag = 0;
                num--;
                writeBuffer.back().push_back({x, y + 1});
                if (stopFlag == 2) {
                    break;
                }
            }
            if (Map[x][y + 1] == 3) {
                return tuple{num, x, y + 1};
            }
        }
        break;
    default:
        break;
    }

    if (countFlag) {
        countDrop += countFlag;
    }
    else {
        countDrop = 0;
    }
    return tuple{num, x, y};
}

// 写Map
void writeMap() {
    for (auto &[x, y] : writeBuffer.back())
        Map[x][y] = 1;
}

// 回溯时恢复Map，把被写成1的2恢复成2
void resetMap(tuple<int, int, int> &state) {
    for (auto &[x, y] : writeBuffer.back())
        Map[x][y] = 2;

    writeBuffer.pop_back();
    states.erase(state);
    statesStack.pop_back();
    solution.pop_back();
}

// i是递归深度，Debug用
bool backtrack(tuple<int, int, int> &state, int i) {
    i++;
    auto [num, x, y] = state;
    vector<Direction> directions{Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};

    // // Debug
    // int size = solution.size();
    // println("{}, {}, {}, {}", num, x, y, i);

    for (auto direction : directions) {

        auto newState = move(num, x, y, direction);
        auto [newNum, newx, newy] = newState;
        // 3的坐标
        if (newx == 13 && newy == 10) {
            // 不剩2了
            if (newNum == 0)
                return true;
            // 还剩2，不可行，回溯
            else {
                writeBuffer.pop_back();
                continue;
            }
        }

        // 20步没撞上2，剪枝
        if (countDrop == 20) {
            writeBuffer.pop_back();
            countDrop--;
            continue;
        }

        // 除撞上3之外，move后state不重复
        if (states.find(newState) == states.end()) {
            states.insert(newState);
            statesStack.push_back(newState);
            solution.push_back(direction);
            writeMap();
            if (backtrack(newState, i))
                return true;
            else
                resetMap(newState);
        }
        // 全局同形
        else
            writeBuffer.pop_back();
    }
    return false;
}

bool solve(tuple<int, int, int> s0) {
    states.insert(s0);
    statesStack.push_back(s0);
    return backtrack(s0, 0);
}

void writeSolution() {
    ofstream outFile("./solution.txt");
    if (!outFile) {
        std::cerr << "ERROR" << std::endl;
        return;
    }
    for (auto i : solution) {
        switch (i) {
        case Direction::UP:
            outFile << "UP" << std::endl;
            break;
        case Direction::DOWN:
            outFile << "DOWN" << std::endl;
            break;
        case Direction::LEFT:
            outFile << "LEFT" << std::endl;
            break;
        case Direction::RIGHT:
            outFile << "RIGHT" << std::endl;
            break;
        default:
            break;
        }
    }
    outFile.close();
}

int main(int argc, char *argv[]) {
    auto num = 0;
    for (auto i : Map)
        for (auto j : i)
            if (j == 2)
                num++;
    tuple s0{num, 3, 10};
    auto result = solve(s0);
    if (result)
        writeSolution();
    return 0;
}