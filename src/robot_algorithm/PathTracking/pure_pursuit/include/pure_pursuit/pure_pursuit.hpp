/*
 Copyright 2024 ROS2 LLC

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#ifndef PURE_PURSUIT__PURE_PURSUIT_HPP_
#define PURE_PURSUIT__PURE_PURSUIT_HPP_

#include <cmath>
#include <vector>
#include <chrono>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "pure_pursuit/logger.hpp"
#include "geometry_msgs/msg/vector3_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "visualization_msgs/msg/marker.hpp"

namespace pure_pursuit
{
struct Vector {
    double vx;
    double vy;
};
struct Pose {
    double x;
    double y;
    double yaw;
};
struct Point {
    double x;   //  位置x
    double y;   //  位置y
};
struct Line {
    Point start;
    Point end;
};

class PurePursuit : public rclcpp::Node
{
public:
    PurePursuit();
    ~PurePursuit();

private:
    void initParam();
    void initPath();
    void initFirstValue();
    double normalizeAngle(double angle);
    void currentPoseCallback(const geometry_msgs::msg::Vector3Stamped::SharedPtr msg);
    void sendVelocity(const double v, const double w);
    void displayCurveOnRviz2();
    void timerCallback();
    
    rclcpp::Subscription<geometry_msgs::msg::Vector3Stamped>::SharedPtr current_pose_;
    double lookaheaddist_{0.0f};
    double k_{0.0f};
    double max_v_{1.0f};
    double max_w_{M_PI_2};
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr vel_;
    rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr marker_pub_;
    rclcpp::TimerBase::SharedPtr timer_;

    // set paths
    std::vector<Line> line_;
    // set constant linear speed
    double v_;
    double w_;





};
}  // namespace pure_pursuit
#endif  // PURE_PURSUIT__PURE_PURSUIT_HPP_
