# World-Coordinate-Transformation<br>
概念：图像处理钟世界坐标系由世界坐标、相机坐标、图像坐标、像素坐标组成。其中世界坐标、相机坐标为3维坐标；图像坐标、像素坐标为2维坐标。<br>
`坐标系之间的转换原理`<br>
1.世界坐标转相机坐标<br>
![Image text](https://github.com/fpeanut/World-Coordinate-Transformation/blob/main/img/1.png)<br>
2.相机坐标转图像坐标<br>
![Image text](https://github.com/fpeanut/World-Coordinate-Transformation/blob/main/img/2.png)<br>
3.图像坐标转像素坐标<br>
![Image text](https://github.com/fpeanut/World-Coordinate-Transformation/blob/main/img/3.png)<br>
4.总的公式推导<br>
![Image text](https://github.com/fpeanut/World-Coordinate-Transformation/blob/main/img/4.png)<br>
 `坐标系转换的相关应用`<br>
应用1:计算相机坐标系与笔记本屏幕3d坐标的关系。
注：该应用是对世界坐标转换功能的演变。即：此处的相机坐标系相当于世界坐标系，屏幕3d坐标系相当于相机坐标系。二者之间存在着旋转变换关系。<br>
流程<br>
![Image text](https://github.com/fpeanut/World-Coordinate-Transformation/blob/main/img/5.jpg)<br>
