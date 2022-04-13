from controller import Robot, Keyboard, Motion
from threading import Thread, Semaphore
import math
import time

from astar import *


class Nao(Robot):
    PHALANX_MAX = 8

    exhibitsGPS = [[0, 0.73], [0, 5], [0, -5],
                   [0, -2], [-5, -2.2], [-4.35, 2.58]]  # GPS of exhibits
    exhibitsPos = [[19, 0], [19, 15], [19, 39],
                   [19, 27], [0, 9], [2, 28]]  # coordinate of exhibits

    def __init__(self):
        Robot.__init__(self)
        self.currentlyPlaying = None  # Robot's current motion
        self.is_arrived = True
        self.posMap = []

        # initialize stuff
        self.loadMap()
        self.findAndEnableDevices()
        self.loadMotionFiles()
        print('Press 1 ~ 7 to start the Nao Robot.')

    def loadMap(self):
        with open("map.txt", "r") as f:
            for line in f:
                tmp = line.split()
                tmp = list(map(int, tmp))
                self.posMap.append(tmp)
            print("Map loaded.")

    def path_search(self, exhibit):
        curPos = self.gps.getValues()
        start_x = -curPos[2] // 0.25 + 20
        start_y = curPos[0] // 0.25 + 20
        path = astar(self.posMap, (int(start_x), int(start_y)), tuple(self.exhibitsPos[exhibit]))
        return path

    def findAndEnableDevices(self):
        # get the time step of the current world.
        self.timeStep = int(self.getBasicTimeStep())

        # camera
        self.cameraTop = self.getDevice("CameraTop")
        self.cameraBottom = self.getDevice("CameraBottom")
        self.cameraTop.enable(4 * self.timeStep)
        self.cameraBottom.enable(4 * self.timeStep)

        # accelerometer
        self.accelerometer = self.getDevice('accelerometer')
        self.accelerometer.enable(4 * self.timeStep)

        # gyro
        self.gyro = self.getDevice('gyro')
        self.gyro.enable(4 * self.timeStep)

        # gps
        self.gps = self.getDevice('gps')
        self.gps.enable(4 * self.timeStep)

        # inertial unit
        self.inertialUnit = self.getDevice('inertial unit')
        self.inertialUnit.enable(self.timeStep)

        # ultrasound sensors
        self.us = []
        usNames = ['Sonar/Left', 'Sonar/Right']
        for i in range(0, len(usNames)):
            self.us.append(self.getDevice(usNames[i]))
            self.us[i].enable(self.timeStep)

        # foot sensors
        self.fsr = []
        fsrNames = ['LFsr', 'RFsr']
        for i in range(0, len(fsrNames)):
            self.fsr.append(self.getDevice(fsrNames[i]))
            self.fsr[i].enable(self.timeStep)

        # foot bumpers
        self.lfootlbumper = self.getDevice('LFoot/Bumper/Left')
        self.lfootrbumper = self.getDevice('LFoot/Bumper/Right')
        self.rfootlbumper = self.getDevice('RFoot/Bumper/Left')
        self.rfootrbumper = self.getDevice('RFoot/Bumper/Right')
        self.lfootlbumper.enable(self.timeStep)
        self.lfootrbumper.enable(self.timeStep)
        self.rfootlbumper.enable(self.timeStep)
        self.rfootrbumper.enable(self.timeStep)

        # there are 7 controlable LED groups in Webots
        self.leds = []
        self.leds.append(self.getDevice('ChestBoard/Led'))
        self.leds.append(self.getDevice('RFoot/Led'))
        self.leds.append(self.getDevice('LFoot/Led'))
        self.leds.append(self.getDevice('Face/Led/Right'))
        self.leds.append(self.getDevice('Face/Led/Left'))
        self.leds.append(self.getDevice('Ears/Led/Right'))
        self.leds.append(self.getDevice('Ears/Led/Left'))

        # get phalanx motor tags
        # the real Nao has only 2 motors for RHand/LHand
        # but in Webots we must implement RHand/LHand with 2x8 motors
        self.lphalanx = []
        self.rphalanx = []
        self.maxPhalanxMotorPosition = []
        self.minPhalanxMotorPosition = []
        for i in range(0, self.PHALANX_MAX):
            self.lphalanx.append(self.getDevice("LPhalanx%d" % (i + 1)))
            self.rphalanx.append(self.getDevice("RPhalanx%d" % (i + 1)))

            # assume right and left hands have the same motor position bounds
            self.maxPhalanxMotorPosition.append(self.rphalanx[i].getMaxPosition())
            self.minPhalanxMotorPosition.append(self.rphalanx[i].getMinPosition())

        # shoulder pitch motors
        self.RShoulderPitch = self.getDevice("RShoulderPitch")
        self.LShoulderPitch = self.getDevice("LShoulderPitch")

        self.LElbowRoll = self.getDevice("LElbowRoll")
        self.RElbowRoll = self.getDevice("RElbowRoll")

        # keyboard
        self.keyboard = self.getKeyboard()
        self.keyboard.enable(10 * self.timeStep)

    def loadMotionFiles(self):
        self.handWave = Motion('../../motions/HandWave.motion')
        self.forwards = Motion('../../motions/Forwards50.motion')
        self.backwards = Motion('../../motions/Backwards.motion')
        self.sideStepLeft = Motion('../../motions/SideStepLeft.motion')
        self.sideStepRight = Motion('../../motions/SideStepRight.motion')
        self.turnLeft40 = Motion('../../motions/TurnLeft40.motion')
        self.turnRight40 = Motion('../../motions/TurnRight40.motion')

    def startMotion(self, motion):
        """
        start doing a motion
        :param motion: motion to act
        :type motion: Motion
        """
        # interrupt current motion
        if self.currentlyPlaying:
            self.currentlyPlaying.stop()

        # start new motion
        motion.play()
        self.currentlyPlaying = motion

        # wait for the action to stop
        if motion == self.forwards:
            time.sleep(1.5)
        if motion == self.sideStepLeft or motion == self.sideStepRight:
            time.sleep(1.8)
        self.currentlyPlaying = None

    def pointFinger(self, mode):
        self.LElbowRoll.setPosition(0)
        self.RElbowRoll.setPosition(0)
        if mode == 'point':
            self.RShoulderPitch.setPosition(0)
            for i in range(3, 6):
                self.rphalanx[i].setPosition(1)
        elif mode == 'reset':
            self.RShoulderPitch.setPosition(1.5)
            for i in range(3, 6):
                self.rphalanx[i].setPosition(0)

    def go(self, exhibit):
        self.path_search(exhibit)
        self.is_arrived = False

        Thread(target=self.goFoward).start()
        Thread(target=self.isArrived, args=(exhibit,)).start()
        Thread(target=self.turn, args=(exhibit,)).start()

    def goFoward(self):
        while not self.is_arrived:
            if self.currentlyPlaying is None:
                while self.us[0].getValue() < 0.7:
                    self.startMotion(self.sideStepRight)
                while self.us[1].getValue() < 0.7:
                    self.startMotion(self.sideStepLeft)

                self.startMotion(self.forwards)

    def isArrived(self, exhibit):
        while not self.is_arrived:
            pos = self.gps.getValues()

            if (pos[0] - self.exhibitsGPS[exhibit][0]) ** 2 + (pos[2] - self.exhibitsGPS[exhibit][1]) ** 2 < 1.5:
                if self.currentlyPlaying:
                    self.currentlyPlaying.stop()
                    self.currentlyPlaying = None

                print(f"Arrived at exhibit {exhibit + 1}")
                self.is_arrived = True
                self.currentlyPlaying = None
                self.pointFinger("point")
                time.sleep(3)
                self.pointFinger("reset")

    def turn(self, exhibit):
        while not self.is_arrived:
            angle = self.getAngle(exhibit)
            # print(angle)

            # modify the heading direction
            if abs(angle) > 0.4:
                if angle > 0:
                    self.startMotion(self.turnLeft40)
                else:
                    self.startMotion(self.turnRight40)

    def getAngle(self, exhibit):
        """
        get angle between heading direction and destination
        :return: angle < 0 when exhibit on the left
        """
        curPos = self.gps.getValues()
        rpy = self.inertialUnit.getRollPitchYaw()
        yawAngle = -rpy[2]  # get yaw angle
        dest = self.exhibitsGPS[exhibit]

        deltaz = dest[1] - curPos[2]
        deltax = dest[0] - curPos[0]
        posAngle = math.atan(deltaz / deltax)
        # print(deltaz, deltax)

        if deltaz < 0 and deltax < 0:
            posAngle -= math.pi
        elif deltaz > 0 and deltax < 0:
            posAngle += math.pi

        # print(f"posangle:{posAngle},yawangle:{yawAngle}")
        angle = yawAngle - posAngle
        return angle

    def run(self):
        print("Ready to go!")
        self.handWave.setLoop(True)
        self.handWave.play()

        # wait until a key is pressed
        prekey = None
        while robot.step(self.timeStep) != -1:  # retrun -1 when Webots terminates the controller
            key = self.keyboard.getKey()
            if key > 0:
                break

        self.handWave.setLoop(False)

        while True:
            key = self.keyboard.getKey()

            if 48 < key < 56:
                exhibit = key - 49
                if prekey != key and not self.is_arrived:
                    self.is_arrived = True
                    self.currentlyPlaying = None
                    print(f"Changing the destination to exhibit {exhibit + 1}...")
                    time.sleep(3)
                else:
                    print(f"Going to exhibit {exhibit + 1}...")

                if prekey == key:
                    continue

                self.go(exhibit)
                prekey = key

            if robot.step(self.timeStep) == -1:
                break


robot = Nao()
robot.run()
