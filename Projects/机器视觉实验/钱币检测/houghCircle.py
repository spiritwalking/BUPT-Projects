import numpy as np
import math
import cv2 as cv
from canny import Canny
from usePackage import rectify_img, show_img
from tqdm import tqdm


def hough_circle(image, orientation, threshold, minr, maxr, region=5):
    h, w = image.shape
    orientation = np.deg2rad(orientation)
    # 限制maxr的大小
    if maxr > int(math.sqrt(h ** 2 + w ** 2)):
        maxr = int(math.sqrt(h ** 2 + w ** 2))

    # 初始化3D投票矩阵
    vote_matrix = np.zeros((maxr, h, w), dtype=np.int32)

    # 每一个edge point下标及方向
    edge_sub = np.argwhere(image)
    edge_orient = np.array([orientation[i, j] for i, j in edge_sub])

    # 并行计算加速
    for r in tqdm(range(minr, maxr)):
        x = np.array([])
        y = np.array([])

        # 数组坐标[i,j]，opencv坐标为[j,i]
        x = np.append(x, edge_sub[:, 0] + r * np.sin(edge_orient))
        x = np.append(x, edge_sub[:, 0] - r * np.sin(edge_orient))
        y = np.append(y, edge_sub[:, 1] + r * np.cos(edge_orient))
        y = np.append(y, edge_sub[:, 1] - r * np.cos(edge_orient))

        # 将坐标拼接起来，以便投票
        hough_coor = np.dstack((x, y))
        hough_coor = hough_coor.reshape(-1, 2).astype(np.uint32)

        for i, j in hough_coor:
            if 0 <= i < h and 0 <= j < w:
                vote_matrix[r, i, j] += 1

    # vote_img = np.sum(vote_matrix, axis=0)
    # vote_img = np.uint8(vote_img * 3)
    # show_img(vote_img)

    circles = []
    votes = []

    # 筛选出适合的圆
    for k in range(math.ceil((maxr - minr) / region)):
        for i in range(math.ceil(h / region)):
            for j in range(math.ceil(w / region)):
                vote = np.sum(vote_matrix[k * region + minr:(k + 1) * region + minr,
                              i * region:(i + 1) * region, j * region:(j + 1) * region])
                if vote > threshold:
                    votes.append(vote)
                    circles.append([i * region + region // 2, j * region + region // 2,
                                    minr + k * region + region // 2])

    final_circles = circles[:]
    # 圆心相邻则选投票数最多的点
    for i in range(len(circles)):
        for j in range(len(circles)):
            x1, y1, r1 = circles[i]
            x2, y2, r2 = circles[j]
            if math.sqrt((x1 - x2) ** 2 + (y1 - y2) ** 2) < 20 and votes[i] < votes[j] and circles[i] in final_circles:
                final_circles.remove(circles[i])

    return final_circles
