import cv2
from pykinect2 import PyKinectV2
from pykinect2.PyKinectV2 import *
from pykinect2 import PyKinectRuntime
import numpy as np

# Initialize Kinect
kinect = PyKinectRuntime.PyKinectRuntime(PyKinectV2.FrameSourceTypes_Color | PyKinectV2.FrameSourceTypes_Depth)
print(type(kinect))
# Get one color picture frame
frame = None
while(True):
	if (kinect.has_new_depth_frame()):
		frame = kinect.get_last_depth_frame()
		break

frame = frame.reshape((424,512))

# maximum = np.amax(frame)
# print maximum
np.save("StableOutput_Depth4",frame)
# frame = (frame/50)


# print(frame.view())

cv2.imwrite("Output_depth.jpg", frame)

kinect.close()

frame = cv2.imread('Output_depth.jpg')

#Create opencv output
output = frame.copy()

#Output to file for testing
cv2.imwrite("Output.jpg", frame)

#56, 182, 241
#20, 77, 144

#Hough circle detection using greyscale img
min_radius = 50
max_radius = 1000

gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
gray_frame = cv2.medianBlur(gray_frame,21)
cv2.imwrite("OutputGray.jpg", gray_frame)

balls = cv2.HoughCircles(gray_frame, cv2.cv.CV_HOUGH_GRADIENT, 1.9, circles=21, minDist=150, param1=100, param2=100, minRadius=100, maxRadius=500)

print("Detected balls")
print(balls)
balls = np.round(balls[0, :]).astype("int")

#Display detected balls
for (x, y, z) in balls:
	cv2.circle(frame, (x, y), z, (0, 255, 0), 4)

cv2.imwrite("DetectedBalls.jpg", frame )
cv2.imshow("output", frame)
cv2.waitKey(0)