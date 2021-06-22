#include "Animator.h"

Animator::Animator() : animationName(""), time(0), index(0), clck(clock()), stop(false) {}

Animator::~Animator() {  }


std::vector<std::string> DivideString(std::string str)
{
	std::stringstream ss(str);
	std::vector<std::string> v;
	while (std::getline(ss, str, ':'))
	{
		v.push_back(str);
	}
	return v;
}

DWORD WINAPI CountTime(LPVOID param)
{
    while (true)
    {
        Animator* animator = (Animator*)param;

        clock_t a = clock();
        animator->time += (double)(a - animator->clck) / CLOCKS_PER_SEC;
        animator->clck = a;
        if (animator->stop)
            return 0;
    }

    return 0;
}