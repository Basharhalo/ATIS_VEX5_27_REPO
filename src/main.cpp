/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Bashar                                                    */
/*    Created:      10/17/2026                                                */
/*    Description:  V5 project - Converted back to VEXcode V5                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// Brain and Controller
brain Brain;
controller Controller1 = controller(primary);

// Drivetrain Motors (Green cartridges = ratio18_1)
motor DT_leftMotor = motor(PORT1, ratio18_1, true);
motor DT_rightMotor = motor(PORT2, ratio18_1, false);

// Claw Grip Motor (Red cartridge = ratio36_1)
motor ClawGrip1_upperMotor = motor(PORT4, ratio36_1, false);

// Claw Arm Motor (Red cartridge = ratio36_1, reversed to match -3 in PROS)
motor ClawArm = motor(PORT3, ratio36_1, true);

// Global variable to store autonomous choice (1 = Left, 2 = Right)
int autonChoice = 1;

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*---------------------------------------------------------------------------*/

void pre_auton(void)
{
    // --- BRAIN SCREEN TOUCHSCREEN SELECTOR ---
    Brain.Screen.clearScreen();
    Brain.Screen.setPenColor(white);
    Brain.Screen.setFont(fontType::mono15);
    Brain.Screen.printAt(20, 20, "SELECT AUTON (Tap Screen):");

    // Draw Left Auton Button (Blue Box) - Drawn ONCE
    Brain.Screen.setFillColor(blue);
    Brain.Screen.drawRectangle(20, 50, 200, 100);
    Brain.Screen.printAt(60, 100, "Left Auton");

    // Draw Right Auton Button (Red Box) - Drawn ONCE
    Brain.Screen.setFillColor(red);
    Brain.Screen.drawRectangle(260, 50, 200, 100);
    Brain.Screen.printAt(300, 100, "Right Auton");

    Brain.Screen.setFillColor(black);
    Brain.Screen.printAt(20, 180, "CURRENT: Left Side Selected");

    // Keep listening for touches until the match starts
    while (true)
    {
        if (Brain.Screen.pressing())
        {
            int x = Brain.Screen.xPosition();
            int y = Brain.Screen.yPosition();

            // Check if touch is inside Left Button box
            if (x >= 20 && x <= 220 && y >= 50 && y <= 150)
            {
                autonChoice = 1;
                Brain.Screen.clearLine(180);
                Brain.Screen.printAt(20, 180, "CURRENT: Left Side Selected");
                wait(250, msec); // Debounce delay
            }
            // Check if touch is inside Right Button box
            if (x >= 260 && x <= 460 && y >= 50 && y <= 150)
            {
                autonChoice = 2;
                Brain.Screen.clearLine(180);
                Brain.Screen.printAt(20, 180, "CURRENT: Right Side Selected");
                wait(250, msec); // Debounce delay
            }
        }
        wait(50, msec); // Prevent CPU hogging
    }
}


/*---------------------------------------------------------------------------*/
/*                             Autonomous Task                               */
/*---------------------------------------------------------------------------*/

void autonomous(void)
{
    switch (autonChoice)
    {
        case 1:
                      DT_leftMotor.spin(forward, 90, percent);
            DT_rightMotor.spin(forward, 90, percent);
            wait(600, msec);

            DT_leftMotor.stop(brake);
            DT_rightMotor.stop(brake);
            wait(200, msec);


            DT_leftMotor.spin(reverse, 95, percent);
            DT_rightMotor.spin(reverse, 95, percent);
            wait(1000, msec);

            DT_leftMotor.spin(forward, 90, percent);
            DT_rightMotor.spin(forward, 90, percent);
            wait(700, msec);

            DT_leftMotor.stop(brake);
            DT_rightMotor.stop(brake);
            wait(200, msec);


            DT_leftMotor.spinFor(reverse, 360, degrees, false);
            DT_rightMotor.spinFor(forward, 360, degrees, true);
            wait(500, msec);

            DT_leftMotor.spin(forward, 90, percent);
            DT_rightMotor.spin(forward, 90, percent);
            wait(500, msec);

            DT_leftMotor.stop(brake);
            DT_rightMotor.stop(brake);
            wait(200, msec);
            break;

        case 2:
            // --- RIGHT SIDE ROUTINE ---
                       DT_leftMotor.spin(forward, 90, percent);
            DT_rightMotor.spin(forward, 90, percent);
            wait(600, msec);

            DT_leftMotor.stop(brake);
            DT_rightMotor.stop(brake);
            wait(200, msec);


            DT_leftMotor.spin(reverse, 95, percent);
            DT_rightMotor.spin(reverse, 95, percent);
            wait(1000, msec);

            DT_leftMotor.spin(forward, 90, percent);
            DT_rightMotor.spin(forward, 90, percent);
            wait(700, msec);

                        DT_leftMotor.stop(brake);
            DT_rightMotor.stop(brake);
            wait(200, msec);


            DT_leftMotor.spinFor(forward, 360, degrees, false);
            DT_rightMotor.spinFor(reverse, 360, degrees, true);
            wait(500, msec);

            DT_leftMotor.spin(forward, 90, percent);
            DT_rightMotor.spin(forward, 90, percent);
            wait(500, msec);

            DT_leftMotor.stop(brake);
            DT_rightMotor.stop(brake);
            wait(200, msec);
            break;



        default:
            break;
    }

    // Safety stop all motors at the end of autonomous
    DT_leftMotor.stop(brake);
    DT_rightMotor.stop(brake);
    ClawArm.stop(hold);
    ClawGrip1_upperMotor.stop(hold);
}

/*---------------------------------------------------------------------------*/
/*                            User Control Task                              */
/*---------------------------------------------------------------------------*/

void usercontrol(void)
{
    while (1)
    {
        // Drivetrain control (Arcade: Left Y for speed, Right X for turn)
        int forwardSpeed = Controller1.Axis3.position();
        int turnSpeed = Controller1.Axis1.position();

        int leftPower = forwardSpeed - turnSpeed;
        int rightPower = forwardSpeed + turnSpeed;

        DT_leftMotor.spin(forward, leftPower, percent);
        DT_rightMotor.spin(forward, rightPower, percent);

        // --------------------------------------------------------------------
        // Claw Grip Control | L2 / R2
        if (Controller1.ButtonL2.pressing())
        {
            ClawGrip1_upperMotor.spin(forward, 100, percent);
        }
        else if (Controller1.ButtonR2.pressing())
        {
            ClawGrip1_upperMotor.spin(reverse, 100, percent);
        }
        else
        {
            ClawGrip1_upperMotor.stop(hold);
        }

        // --------------------------------------------------------------------
        // Claw Arm Control | R1 / L1
        if (Controller1.ButtonR1.pressing())
        {
            ClawArm.spin(forward, 100, percent);
        }
        else if (Controller1.ButtonL1.pressing())
        {
            ClawArm.spin(reverse, 100, percent);
        }
        else
        {
            ClawArm.stop(hold);
        }

        wait(20, msec); // Sleep to prevent wasted CPU resources
    }
}

/*---------------------------------------------------------------------------*/
/*                               Main Setup                                  */
/*---------------------------------------------------------------------------*/

int main()
{
    // Set up callbacks for autonomous and driver control periods
    Competition.autonomous(autonomous);
    Competition.drivercontrol(usercontrol);

    // Run the pre-autonomous touchscreen selector function
    pre_auton();

    // Prevent main from exiting with an infinite loop
    while (true)
    {
        wait(100, msec);
    }
}
