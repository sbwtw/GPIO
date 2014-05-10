
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>

#include "gpio.h"

bool GPIO::exist(unsigned gpio) {
    struct stat buff;
    std::stringstream ss;
    ss << "/sys/class/gpio/gpio";
    ss << gpio;
    
    if (0 == lstat(ss.str().c_str(), &buff)) 
        return true;

    return false;
}

bool GPIO::value() const {
    std::stringstream ss;
    ss << "/sys/class/gpio/gpio";
    ss << this->port;
    ss << "/value";

    std::ifstream in(ss.str().c_str());

    if (in.fail()) {
        std::cout << "read gpio" << this->port
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

GPIO::DIRECTION GPIO::direction() const {
    std::stringstream ss;
    ss << "/sys/class/gpio/gpio";
    ss << this->port;
    ss << "/direction";

    std::ifstream in(ss.str().c_str());

    if (in.fail()) {
        std::cout << "read gpio" << this->port
                    << " Direction fail!" << std::endl;
        return DIRECTION::IN;
    }

    std::string str;

    in >> str;
    in.close();

    if (str == "in")
        return DIRECTION::IN;
    else 
        return DIRECTION::OUT;
}

void GPIO::setDirection(DIRECTION dir) {
    std::stringstream ss;
    ss << "/sys/class/gpio/gpio";
    ss << this->port;
    ss << "/direction";

    std::ofstream out(ss.str().c_str());

    if (out.fail()) {
        std::cout << "set gpio" << this->port
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

    if (port == this->port)
        return ;

    this->unexportPort();
    this->port = port;
    this->exportPort();
    this->setDirection(this->dir);
}

void GPIO::down(unsigned times) const {

    while (times--) {
        this->high();
        this->low();
        usleep(1);
    }
}

void GPIO::pulse(unsigned lowWidth, unsigned highWidth, unsigned times) const {

    while (times--) {
        this->low();
        usleep(lowWidth);
        this->high();
        usleep(highWidth);
    }
}

void GPIO::up(unsigned times) const {
    
    while (times--) {
        this->low();
        this->high();
        usleep(1);
    }
}

void GPIO::high() const {
    std::stringstream ss;
    ss << "/sys/class/gpio/gpio";
    ss << this->port;
    ss << "/value";

    std::ofstream out(ss.str().c_str());

    if (out.fail()) {
        std::cout << "set gpio" << this->port 
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
        std::cout << "set gpio" << this->port 
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
        std::cout << "unexport GPIO" << this->port << " fail!" << std::endl;
        return ;
    }

    out << this->port;
    out.close();
}

void GPIO::exportPort() const {

    if (this->exist(this->port)) {
        std::cout << "Warning: GPIO" << this->port << " already setup!" << std::endl;
        return ;
    }

    std::string str("/sys/class/gpio/export");

    std::ofstream out(str.c_str());

    if (out.fail()) {
        std::cout << "export GPIO" << this->port << " fail!" << std::endl;
        return ;
    }

    out << this->port;
    out.close();
}

void GPIO::set(unsigned port, DIRECTION dir) {
    this->setDirection(dir);
    this->setPort(port);
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

