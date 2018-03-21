// *****************************************************************************
//
// Copyright (c) 2015, Southwest Research Institute® (SwRI®)
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Southwest Research Institute® (SwRI®) nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// *****************************************************************************

#ifndef MAPVIZ_PLUGINS_POINTCLOUD2_PLUGIN_H_
#define MAPVIZ_PLUGINS_POINTCLOUD2_PLUGIN_H_

// C++ standard libraries
#include <string>
#include <deque>
#include <vector>
#include <map>

#include <mapviz/mapviz_plugin.h>

// QT libraries
#include <QGLWidget>
#include <QColor>
#include <QMutex>

// ROS libraries
#include <sensor_msgs/PointCloud2.h>

// QT autogenerated files
#include "ui_pointcloud2_config.h"

namespace mapviz_plugins
{
  class PointCloud2Plugin : public mapviz::MapvizPlugin
  {
  Q_OBJECT

  public:
    struct FieldInfo
    {
      uint8_t datatype;
      uint32_t offset;
    };

    enum
    {
      COLOR_FLAT = 0,
      COLOR_Z = 3
    };

    PointCloud2Plugin();
    virtual ~PointCloud2Plugin();

    bool Initialize(QGLWidget* canvas);
    void Shutdown()
    {
    }

    void ClearHistory();

    void Draw(double x, double y, double scale);

    void Transform();

    void LoadConfig(const YAML::Node& node, const std::string& path);
    void SaveConfig(YAML::Emitter& emitter, const std::string& path);

    QWidget* GetConfigWidget(QWidget* parent);

  protected:
    void PrintError(const std::string& message);
    void PrintInfo(const std::string& message);
    void PrintWarning(const std::string& message);

  protected Q_SLOTS:
    void SelectTopic();
    void TopicEdited();
    void AlphaEdited();
    void ColorTransformerChanged(int index);
    void MinValueChanged(double value);
    void MaxValueChanged(double value);
    void PointSizeChanged(int value);
    void BufferSizeChanged(int value);
    void UseRainbowChanged(int check_state);
    void UseAutomaxminChanged(int check_state);
    void UpdateColors();
    void DrawIcon();
    void ResetTransformedPointClouds();

  private:
    struct StampedPoint
    {
      tf::Point point;
      tf::Point transformed_point;
      QColor color;
      std::vector<double> features;
    };

    struct Scan
    {
      ros::Time stamp;
      QColor color;
      std::deque<StampedPoint> points;
      std::string source_frame;
      bool transformed;
      std::map<std::string, FieldInfo> new_features;
    };

    double PointFeature(const uint8_t*, const FieldInfo&);
    void PointCloud2Callback(const sensor_msgs::PointCloud2ConstPtr& scan);
    QColor CalculateColor(const StampedPoint& point);
    void UpdateMinMaxWidgets();

    Ui::PointCloud2_config ui_;
    QWidget* config_widget_;

    std::string topic_;
    double alpha_;
    double max_value_;
    double min_value_;
    size_t point_size_;
    size_t buffer_size_;
    bool new_topic_;
    bool has_message_;
    size_t num_of_feats_;
    bool need_new_list_;
    std::string saved_color_transformer_;
    bool need_minmax_;
    std::vector<double> max_;
    std::vector<double> min_;
    // Use a list instead of a deque for scans to facilitate removing
    // timed-out scans in the middle of the list in case I ever re-implement
    // decay time (evenator)
    std::deque<Scan> scans_;
    ros::Subscriber pc2_sub_;

    QMutex scan_mutex_;
  };
}

#endif  // MAPVIZ_PLUGINS_LASERSCAN_PLUGIN_H_
