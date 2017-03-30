#include "kun.h"
#include "engine.h"
#include <chrono>


int main ()
{
	Engine* const engine = new Engine ();

	std::chrono::steady_clock::time_point time_begin = std::chrono::steady_clock::now ();

	double current_time = 0.0f;
	double accumulator = 0.0f;
	bool want_exit = false;

	while (!want_exit)
	{
		std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now ();
		std::chrono::duration<double> time_diff = time_end - time_begin;

		const double new_time = time_diff.count ();
		double delta_time = new_time - current_time;
		current_time = new_time;

		if (delta_time > 0.25f)
			delta_time = 0.25f;


		accumulator += delta_time;

		const double time = 1.0 / 60.0;

		while (accumulator >= time)
		{
			engine->Update (time);
			accumulator -= time;
		}

		engine->Render ();
	}

	delete engine;

	return 0;
}

