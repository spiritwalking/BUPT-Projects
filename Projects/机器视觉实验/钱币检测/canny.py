import cv2 as cv
import numpy as np

from usePackage import show_img, rectify_img


class Canny:
    def __init__(self, image, kernal_size, threshold1, threshold2):
        """
        :param image: 输入的图片
        :param kernal_size: 高斯核大小
        :param threshold1: 双阈值法中的高阈值
        :param threshold2: 双阈值法中的低阈值
        """
        self.ori_image = image
        self.image = image
        self.kernal_size = kernal_size
        self.threshold1 = threshold1
        self.threshold2 = threshold2
        self.height, self.width = image.shape

        self.orientation = None
        self.magnitude = None

    def grad_magnitude(self):
        """
        calculate magnitude and orientation of gradient
        """
        dx = cv.Sobel(self.image, cv.CV_32F, 1, 0, ksize=3)
        dy = cv.Sobel(self.image, cv.CV_32F, 0, 1, ksize=3)

        self.magnitude = cv.magnitude(dx, dy).astype(np.uint8)
        self.image = self.magnitude
        self.orientation = cv.phase(dx, dy, angleInDegrees=True)
        # show_img(self.magnitude)

    def Non_maximum_suppression(self):
        self.orientation[self.orientation >= 180] -= 180
        a, b = 0, 0

        for i in range(1, self.height - 1):
            for j in range(1, self.width - 1):
                if self.orientation[i][j] < 22.5 or self.orientation[i][j] >= 157.5:
                    a, b = self.image[i][j + 1], self.image[i][j - 1]
                elif 22.5 <= self.orientation[i][j] < 67.5:
                    a, b = self.image[i + 1][j + 1], self.image[i - 1][j - 1]
                elif 67.5 <= self.orientation[i][j] < 112.5:
                    a, b = self.image[i + 1][j], self.image[i - 1][j]
                elif 112.5 <= self.orientation[i][j] < 157.5:
                    a, b = self.image[i - 1][j + 1], self.image[i + 1][j - 1]

                if self.image[i][j] < a or self.image[i][j] < b:
                    self.image[i][j] = 0

        # show_img(self.image)

    def Hysteresis_thresholding(self):
        thresholded = np.zeros((self.height, self.width))

        index = self.magnitude >= self.threshold1
        thresholded[index] = 1

        index = (self.threshold2 < self.magnitude) & (self.magnitude < self.threshold1)
        thresholded[index] = -1

        for i in range(1, self.height - 1):
            for j in range(1, self.width - 1):
                if thresholded[i][j] == -1:
                    if sum([thresholded[x][y] for x in range(i - 1, i + 2) for y in range(j - 1, j + 2)]) > -1:
                        thresholded[i][j] = 1
                    else:
                        thresholded[i][j] = 0

        self.image = np.uint8(thresholded) * 255

    def algorithm(self):
        # Filter image with Gaussian Blur
        self.image = cv.GaussianBlur(self.image, (self.kernal_size, self.kernal_size), 0)
        # Find magnitude and orientation of gradient
        self.grad_magnitude()
        # Thin wide edges down to single pixel width
        self.Non_maximum_suppression()
        # Linking and thresholding
        self.Hysteresis_thresholding()
        return self.image


if __name__ == "__main__":
    image = cv.imread("images/picture.jpg", 0)
    image = cv.resize(image, (int(image.shape[1] / 4), int(image.shape[0] / 4)))

    canny = Canny(image, 5, 100, 60)
    edge_img = canny.algorithm()
    show_img(edge_img)

    # 使用opencv霍夫变换测试
    circles = cv.HoughCircles(edge_img, cv.HOUGH_GRADIENT, 1, 100, param1=10, param2=10, minRadius=10, maxRadius=95)
    circles = np.uint16(np.around(circles))

    for x, y, r in circles[0, :]:
        cv.circle(image, (x, y), r, (0, 255, 0), 2)

    show_img(image)
