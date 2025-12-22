#include "jisuan/jisuan.h"

static LXjiegou all_change(LXjiegou jiegou, double k)  //整体尺寸的调整，对应工作频率的变化
{
	jiegou.Ra = jiegou.Ra * k;
	jiegou.Rb = jiegou.Rb * k;
	jiegou.Rc = jiegou.Rc * k;
	jiegou.Rg = jiegou.Rg * k;
	jiegou.L = jiegou.L * k;
	jiegou.del = jiegou.del * k;
	return jiegou;
};
