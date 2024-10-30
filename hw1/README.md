# 旋转与投影

> 作业链接：https://games-cn.org/forums/topic/graphics-intro-hw1

> 课件：https://sites.cs.ucsb.edu/~lingqi/teaching/resources/GAMES101_Lecture_04.pdf

## `get_model_matrix`

构建模型变换矩阵，实现三维中绕$z$轴旋转的变换矩阵。课件中有写好的绕$z$轴旋转的旋转矩阵。
在`eigen`创建的矩阵中填上去即可。熟悉一下`eigen`库怎么操作。

## `get_projection_matrix`

构建透视投影矩阵。按照课件上的步骤，透视投影分成两个步骤，先将透视投影（Perspective Projection）转换成正交投影（Orthographic Projection），
然后再进行正交投影。第一步就是计算从Frustum到Cuboid的变换矩阵$M_{persp\to ortho}$,
然后再把这个和正交投影矩阵$M_{ortho}$乘起来，这两个矩阵都在课件中给出了。

$$
M_{persp}=M_{ortho}M_{persp\to ortho}
$$

这里需要处理的是该函数的输入，与课件中公式使用的变量并不一致，需要将其进行转换。
转换的部分在课件 https://sites.cs.ucsb.edu/~lingqi/teaching/resources/GAMES101_Lecture_05.pdf
中有提到。

这里`eye_fov`指的是$fovY$，根据课件，有下面的关系

$$
\tan \frac{fovY}{2} = \frac{t}{|n|},
aspect=\frac{r}{t}
$$

那么就能通过$n,f$计算得到$l,r,t,b$了：
$$
t=|n|\tan \frac{fovY}{2},b=-t
\\
r=aspect\times t,l=-r
$$

## `get_rotation`

这个矩阵在课件中也有介绍，使用Rodrigues' Rotation Formula
就可以算出来：

$$
\mathbf{R}(\mathbf{n},\alpha)
=
\cos (\alpha) \mathbf{I}
+
(1-\cos(\alpha)) \mathbf{n}\mathbf{n}^\top
+
\sin(\alpha)

\begin{pmatrix}
0 & -n_z & n_y
\\
n_z & 0 & -n_x
\\
-n_y & n_x & 0
\end{pmatrix}
$$