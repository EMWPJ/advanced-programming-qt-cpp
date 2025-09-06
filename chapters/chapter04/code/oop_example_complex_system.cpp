#include <iostream>
#include <vector>
#include <string>

// 组件：传感器
class Sensor {
public:
    Sensor(const std::string& type) : type(type) {}
    void read() const {
        std::cout << "传感器[" << type << "] 正在读取数据。" << std::endl;
    }
private:
    std::string type;
};

// 组件：执行器
class Actuator {
public:
    Actuator(const std::string& type) : type(type) {}
    void activate() const {
        std::cout << "执行器[" << type << "] 被激活。" << std::endl;
    }
private:
    std::string type;
};

// 子系统：机器人手臂
class RobotArm {
public:
    RobotArm(const std::string& name) : name(name) {}
    void addActuator(const Actuator& actuator) {
        actuators.push_back(actuator);
    }
    void move() const {
        std::cout << "机器人手臂[" << name << "] 开始移动。" << std::endl;
        for (const auto& a : actuators) {
            a.activate();
        }
    }
private:
    std::string name;
    std::vector<Actuator> actuators;
};

// 子系统：机器人传感系统
class RobotSensorSystem {
public:
    void addSensor(const Sensor& sensor) {
        sensors.push_back(sensor);
    }
    void scan() const {
        std::cout << "机器人传感系统开始扫描环境。" << std::endl;
        for (const auto& s : sensors) {
            s.read();
        }
    }
private:
    std::vector<Sensor> sensors;
};

// 复杂系统：机器人
class Robot {
public:
    Robot(const std::string& name) : name(name) {}
    void addArm(const RobotArm& arm) {
        arms.push_back(arm);
    }
    void setSensorSystem(const RobotSensorSystem& sensorSystem) {
        this->sensorSystem = sensorSystem;
    }
    void operate() const {
        std::cout << "机器人[" << name << "] 启动。" << std::endl;
        sensorSystem.scan();
        for (const auto& arm : arms) {
            arm.move();
        }
        std::cout << "机器人[" << name << "] 操作完成。" << std::endl;
    }
private:
    std::string name;
    std::vector<RobotArm> arms;
    RobotSensorSystem sensorSystem;
};

int main() {
    // 创建传感器和执行器
    Sensor tempSensor("温度");
    Sensor camSensor("摄像头");
    Actuator motor("电机");
    Actuator gripper("夹爪");

    // 创建手臂并添加执行器
    RobotArm leftArm("左臂");
    leftArm.addActuator(motor);
    leftArm.addActuator(gripper);

    // 创建传感系统并添加传感器
    RobotSensorSystem sensorSystem;
    sensorSystem.addSensor(tempSensor);
    sensorSystem.addSensor(camSensor);

    // 创建机器人并组装
    Robot robot("Alpha");
    robot.addArm(leftArm);
    robot.setSensorSystem(sensorSystem);

    // 启动机器人
    robot.operate();

    std::cin.get();
    return 0;
}
