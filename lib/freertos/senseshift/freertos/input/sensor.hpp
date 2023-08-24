#pragma once

#include <senseshift/freertos/task.hpp>
#include <senseshift/input/sensor.hpp>

#include <cstdint>

namespace SenseShift::FreeRTOS::Input {
    /**
     * Sensor update task
     */
    class SensorUpdateTask : public Task<SensorUpdateTask> {
        friend class Task<SensorUpdateTask>;

      private:
        using Sensor_t = ::SenseShift::Input::IMemoizedSensor;

      public:
        SensorUpdateTask(Sensor_t* sensor, std::uint32_t updateDelay, TaskConfig taskConfig) :
          Task<SensorUpdateTask>(taskConfig), updateDelay(updateDelay){};

      protected:
        Sensor_t* sensor;
        std::uint32_t updateDelay;

        void run()
        {
            while (true) {
                this->sensor->updateValue();
                ::delay(this->updateDelay);
            }
        }
    };

    template<typename _Tp>
    class TaskedSensor : public SensorUpdateTask, public ::SenseShift::Input::ISensor<_Tp> {
        friend class SensorUpdateTask;

      private:
        using Sensor_t = ::SenseShift::Input::MemoizedSensor<_Tp>;

      public:
        TaskedSensor(Sensor_t* sensor, std::uint32_t updateDelay, TaskConfig taskConfig) :
          SensorUpdateTask(sensor, updateDelay, taskConfig), sensor(sensor){};

        void begin() override
        {
            this->setup();
            SensorUpdateTask::begin();
        };

        void setup() override { this->sensor->setup(); };

        _Tp getValue() override { return this->sensor->getValue(); };

      private:
        Sensor_t* sensor;
    };
} // namespace SenseShift::FreeRTOS::Input