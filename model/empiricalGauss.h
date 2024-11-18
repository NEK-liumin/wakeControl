#ifndef EMPIRICALGAUSS_H
#define EMPIRICALGAUSS_H

#include "model.h"

// 经验高斯模型
// 其中参数需要用户事先给定
// 例如sigma_0D。我猜，是不偏转情况下的初始无量纲尾流宽度
// 与其他高斯模型不同，上面的两个参数是定义在x = 0处的，而非x = x0处。

class EmpiricalGauss :public Model
{

};

#endif // !EMPIRICALGAUSS_H
