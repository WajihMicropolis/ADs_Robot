#include "ODrive.hpp"

ODrive::ODrive(/* args */)
{
    this->odrive = new ODriveArduino(Serial1);
}

void ODrive::Init()
{
    Serial1.begin(BAUDRATE, SERIAL_8N1, ESP32_UART2_PIN_TX, ESP32_UART2_PIN_RX);
    this->SetSpeed(0.0, 0.0);
    Serial.println("[ODRIVE] ODrive communication initialized!");
}

void ODrive::motorControl(int RC_Throttle, int RC_Steering)
{
    float throttle = 0,
          steering = 0;

    if (RC_Throttle != 0)
    {
        throttle = map(abs(RC_Throttle), minThrPer, maxThrPer, minMotorSpeed, maxMotorFwSpeed);
        throttle = throttle / 100 * (RC_Throttle > 0 ? 1 : -1);
    }
    else
        throttle = 0;

    if (RC_Steering != 0)
    {
        steering = map(abs(RC_Steering), minSteerPer, maxSteerPer, minMotorSpeed, maxSteerspeed);
        steering = steering / 100 * (RC_Steering > 0 ? 1 : -1);
    }
    else
        steering = 0;

    this->SetSpeed(throttle, steering);
}

void ODrive::SetSpeed(float Linear_x, float Angle_z)
{
    float Center_Of_Rotation,
        Left_Wheel_Velocity,
        Right_Wheel_Velocity,
        Left_Wheel_Velocity_In_RPS,
        Right_Wheel_Velocity_In_RPS;

    if (Angle_z != 0.0)
    {
        Center_Of_Rotation = Linear_x / Angle_z;

        // Formula for converting the velocity cmd twist msg to motor velocity
        Left_Wheel_Velocity = -1 * (Angle_z * (Center_Of_Rotation - WheelBase / 2));
        Right_Wheel_Velocity = Angle_z * (Center_Of_Rotation + WheelBase / 2);

        Left_Wheel_Velocity_In_RPS = Left_Wheel_Velocity / (2 * 3.14 * WheelRadius);
        Right_Wheel_Velocity_In_RPS = Right_Wheel_Velocity / (2 * 3.14 * WheelRadius);
    }
    else
    {
        Left_Wheel_Velocity_In_RPS = Right_Wheel_Velocity_In_RPS = Linear_x / (2 * 3.14 * WheelRadius);
        Left_Wheel_Velocity_In_RPS = -1 * (Left_Wheel_Velocity_In_RPS);
    }
    // Serial.println("RightMotor" + String(Right_Wheel_Velocity_In_RPS));
    // Serial.println("LeftMotor" + String(Left_Wheel_Velocity_In_RPS));
    this->odrive->SetVelocity(RightMotor, Right_Wheel_Velocity_In_RPS);
    this->odrive->SetVelocity(LeftMotor, Left_Wheel_Velocity_In_RPS);
}

void ODrive::SerialControl()
{
    if (Serial.available())
    {
        char c = Serial.read();
        Serial.println((String) " Serial.Read(): " + c);

        // Run calibration sequence
        if (c == '0' || c == '1')
        {
            int requested_state = AxisState::AXIS_STATE_CLOSED_LOOP_CONTROL;

            Serial << "Axis" << c << ": Requesting state " << requested_state << '\n';
            this->odrive->run_state(0, requested_state, false); // don't wait

            Serial << "Axis" << c << ": Requesting state " << requested_state << '\n';
            this->odrive->run_state(1, requested_state, false); // don't wait
        }

        else if (c == 'f')
        {
            this->velocity0 += 0.1;
            this->velocity1 -= 0.1;
            Serial.printf("Velocity is set to: %f", this->velocity0);

            this->odrive->SetVelocity(0, this->velocity0);
            this->odrive->SetVelocity(1, this->velocity1);
        }

        else if (c == 'r')
        {
            this->velocity0 -= 0.1;
            this->velocity1 += 0.1;
            Serial.printf("Velocity is set to: %f", this->velocity0);

            this->odrive->SetVelocity(0, this->velocity0);
            this->odrive->SetVelocity(1, this->velocity1);
        }

        // Read bus voltage
        if (c == 'b')
        {
            // odrive_serial << "r vbus_voltage\n";
            Serial1 << "r vbus_voltage\n";
            Serial << "Vbus voltage: " << this->odrive->readFloat() << '\n';
        }
    }
}

ODrive::~ODrive()
{
}