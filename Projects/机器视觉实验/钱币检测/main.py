import cv2 as cv
import numpy as np
import time

from canny import Canny
from houghCircle import hough_circle
from usePackage import rectify_img, show_img

if __name__ == "__main__":
    image = cv.imread("images/picture.jpg")
    resized_img = rectify_img(image, 850)
    # 转换成灰度图
    gray_img = cv.cvtColor(resized_img, cv.COLOR_BGR2GRAY)

    # canny算法
    print("Canny Algorithm...")
    canny = Canny(gray_img, 5, 100, 60)
    edge_img = canny.algorithm()
    # show_img(edge_img)

    # 霍夫变换
    print("Hough Transform...")
    circles = hough_circle(edge_img, canny.orientation, 300, 30, 60)
    for x, y, r in circles:
        print(f"Circle center :({y}, {x}), Radius: {r}")
        cv.circle(resized_img, (y, x), r, (0, 255, 0), 2)

    show_img(resized_img)
