import cv2
import numpy as np

background = cv2.imread('Background_Color.jpg')
frame = cv2.imread('StableOutput_Color1.jpg')

diff = cv2.absdiff(frame, background)
cv2.imwrite("diff.jpg", diff)

#Hough circle detection using greyscale img
min_radius = 50
max_radius = 1000

# gray_frame = cv2.cvtColor(diff, cv2.COLOR_BGR2GRAY)


lower_blue = np.array([15, 15, 15])
upper_blue = np.array([255, 255, 255])

gray_frame = cv2.inRange(diff, lower_blue, upper_blue)
gray_frame = cv2.medianBlur(gray_frame, 9)

cv2.imwrite("OutputGray.jpg", gray_frame)

cv2.imwrite("gray.jpg", gray_frame)

masked_frame = cv2.bitwise_and(frame, frame, mask = gray_frame)

cv2.imwrite("masked.jpg", masked_frame)

gray_frame = cv2.inRange(masked_frame, lower_blue, upper_blue)
gray_frame = cv2.medianBlur(gray_frame,3)
cv2.imwrite("OutputGray.jpg", gray_frame)

balls = cv2.HoughCircles(gray_frame, cv2.cv.CV_HOUGH_GRADIENT, 5, minDist=40, param1=100, param2=60, minRadius=17, maxRadius=30)

detector = cv2.SimpleBlobDetector()

keypoints = detector.detect(gray_frame)

print(keypoints)

im_with_keypoints = cv2.drawKeypoints(frame, keypoints, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

cv2.imwrite("blobballs.jpg", im_with_keypoints)

print("Detected balls:") 
print(balls)
balls = np.round(balls[0, :]).astype("int")

#Display detected balls
for (x, y, z) in balls:
	cv2.circle(diff, (x, y), z, (0, 255, 0), 4)

cv2.imwrite("DetectedBalls.jpg", diff)
