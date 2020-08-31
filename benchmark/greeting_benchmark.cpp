#include <project_template/greeting.hpp>
#include <benchmark/benchmark.h>

namespace {
	void benchmark_greeting(benchmark::State& state)
	{
		for ([[maybe_unused]] auto _ : state) {
			auto const greeting = project_template::greet("project_template");
			benchmark::DoNotOptimize(greeting);
		}
	}

	BENCHMARK(benchmark_greeting); // NOLINT(cert-err58-cpp)
} // namespace

BENCHMARK_MAIN();
