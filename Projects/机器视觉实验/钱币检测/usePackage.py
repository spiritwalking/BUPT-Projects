import cv2 as cv
import numpy as np


def show_img(img):
    """展示图片"""
    cv.imshow('img', img)
    cv.waitKey(0)
    cv.destroyAllWindows()


def rectify_img(img, size):
    """
    将图片调整为合适的大小
    :param img: 原图片
    :param size: 缩放大小
    :return: 缩放后的图片
    """
    height, width, _ = img.shape
    factor = height if height > width else width
    factor = size / factor
    img_resized = cv.resize(img, (int(width * factor), int(height * factor)))
    return img_resized


if __name__ == "__main__":
    # 以BGR格式读入图像
    img = cv.imread('images/picture.jpg')

    # 将图像调整为合适的大小
    resized_img = rectify_img(img, 500)

    # 转换成灰度图
    gray_img = cv.cvtColor(resized_img, cv.COLOR_BGR2GRAY)

    # 高斯滤波降噪
    gaussian_img = cv.GaussianBlur(gray_img, (3, 3), 0)

    # 利用Canny进行边缘检测
    edge_img = cv.Canny(gaussian_img, 90, 180, apertureSize=3)
    cv.imshow('edge_img', edge_img)
    cv.waitKey(0)
    cv.destroyAllWindows()

    # 霍夫变换检测圆
    circles = cv.HoughCircles(edge_img, cv.HOUGH_GRADIENT, 1, 100, param1=90, param2=25, minRadius=10, maxRadius=95)
    circles = np.uint16(np.around(circles))

    # 以[[x1,y1,r1],[x2,y2,r2]...]格式存储圆的参数
    for x, y, r in circles[0, :]:
        cv.circle(resized_img, (x, y), r, (0, 255, 0), 2)

    cv.imshow('output', resized_img)
    cv.waitKey(0)
    cv.destroyAllWindows()
