
#ifndef GPIO_H
#define GPIO_H

#include <fstream>
#include <string>
#include <sstream>

class GPIO {
public:
    enum DIRECTION {IN, OUT};

protected:
    unsigned port;
    DIRECTION dir;

public:
    GPIO(unsigned port, DIRECTION dir);
    ~GPIO();

    // 设置
    void set(unsigned port, DIRECTION dir);
    // 设置方向
    void setDirection(DIRECTION dir);
    // 设置端口
    void setPort(unsigned port);

    // 设置高低电位
    void low() const;
    void high() const;

    // 产生上升/下降沿
    void up(unsigned times = 1) const;
    void down(unsigned times = 1) const;

    // 读取高低电位
    bool value() const;
    // 读取方向
    DIRECTION direction() const;

protected:
    // 导出
    void exportPort() const;
    void unexportPort() const;
};

#endif // GPIO_H
