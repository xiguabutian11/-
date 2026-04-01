#include "jisuan/jisuan.h"

static LXjiegou all_change(LXjiegou jiegou, double k)  //整体尺寸的调整，对应工作频率的变化
{
	jiegou.Ra = jiegou.Ra * k;
	jiegou.Rb = jiegou.Rb * k;
	if(jiegou.Rb- jiegou.Ra<= 0.0001){
		std::cout << "螺旋线厚度小于0.1mm,固定为0.1mm";
		jiegou.Rb = jiegou.Ra + 0.0001001;
	}
	jiegou.Rc = jiegou.Rc * k;
	jiegou.Rg = jiegou.Rg * k;
	jiegou.L = jiegou.L * k;
	jiegou.del = jiegou.del * k;
	return jiegou;
};
