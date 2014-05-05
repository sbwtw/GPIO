
#include <iostream>
#include <unistd.h>

#include "gpio.h"

bool GPIO::value() const {
    std::stringstream ss;
    ss << "/sys/class/gpio/gpio";
    ss << this->port;
    ss << "/value";

    std::ifstream in(ss.str().c_str());

    if (in.fail()) {
        std::cout << "read gpio " << this->port
                    << " value fail!" << std::endl;
        return false;
    }

    unsigned value;
    in >> value;
    in.close();

    if (value)
        return true;
    else 
        return false;
}

void GPIO::setDirection(DIRECTION dir) {
    std::stringstream ss;
    ss << "/sys/class/gpio/gpio";
    ss << this->port;
    ss << "/direction";

    std::ofstream out(ss.str().c_str());

    if (out.fail()) {
        std::cout << "set gpio " << this->port
                    << " Direction fail!" << std::endl;
        return ;
    }

    if (dir == IN)
        out << "in";
    else
        out << "out";

    out.close();
}

void GPIO::setPort(unsigned port) {
    this->unexportPort();
    this->port = port;
    this->exportPort();
    this->setDirection(this->dir);
}

void GPIO::down(unsigned times) const {
    unsigned count(0);

    do {
        this->high();
        this->low();
        usleep(1);
    } while (++count != times);
}

void GPIO::up(unsigned times) const {
    unsigned count(0);
    
    do {
        this->low();
        this->high();
        usleep(1);
    } while (++count != times);
}

void GPIO::high() const {
    std::stringstream ss;
    ss << "/sys/class/gpio/gpio";
    ss << this->port;
    ss << "/value";

    std::ofstream out(ss.str().c_str());

    if (out.fail()) {
        std::cout << "set gpio " << this->port 
                    << " to high fail!" << std::endl;
        return ;
    }

    out << 1;
    out.close();
}

void GPIO::low() const {
    std::stringstream ss;
    ss << "/sys/class/gpio/gpio";
    ss << this->port;
    ss << "/value";

    std::ofstream out(ss.str().c_str());

    if (out.fail()) {
        std::cout << "set gpio " << this->port 
                    << " to low fail!" << std::endl;
        return ;
    }

    out << 0;
    out.close();
}

void GPIO::unexportPort() const {
    std::string str("/sys/class/gpio/unexport");

    std::ofstream out(str.c_str());

    if (out.fail()) {
        std::cout << "unexportPort fail!" << std::endl;
        return ;
    }

    out << this->port;
    out.close();
}

void GPIO::exportPort() const {
    std::string str("/sys/class/gpio/export");

    std::ofstream out(str.c_str());

    if (out.fail()) {
        std::cout << "exportPort fail!" << std::endl;
        return ;
    }

    out << this->port;
    out.close();
}

void GPIO::set(unsigned port, DIRECTION dir) {
    this->setPort(port);
    this->setDirection(dir);
}

GPIO::~GPIO() {
    this->unexportPort();
}

GPIO::GPIO(unsigned gpio, DIRECTION dir) {
    this->port = gpio;
    this->dir = dir;

    this->exportPort();
    this->setDirection(dir);
}

