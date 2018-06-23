#ifndef AREA51_BENCHMARK_V2_HPP
#define AREA51_BENCHMARK_V2_HPP

#include <tuple>
#include <array>
#include <chrono>
#include <utility>
#include <vector>
#include <stdexcept>
#include <algorithm>

namespace shino
{

    template <typename QualifiedType, typename OriginalType>
    using enable_sfinae = std::enable_if_t<std::is_same_v<std::decay_t<QualifiedType>, OriginalType>>;

    template <typename T>
    struct is_straight_tuple : public std::false_type
    {};

    template <typename ... Ts>
    struct is_straight_tuple<std::tuple<Ts...>> : public std::true_type
    {};

    template <typename T>
    struct is_std_array : public std::false_type
    {};

    template <typename T, std::size_t size>
    struct is_std_array<std::array<T, size>> : public std::true_type
    {};

    template <typename T>
    struct is_tuple_like : public std::bool_constant<is_std_array<T>::value || is_straight_tuple<T>::value>
    {};

    namespace detail
    {
        template <typename Callable, typename Tuple>
        decltype(auto) genuine_apply(Callable&& callable, Tuple&& tuple, std::true_type)
        {
            return std::apply(std::forward<Callable>(callable), std::forward<Tuple>(tuple));
        }

        template <typename Callable, typename Tuple>
        decltype(auto) genuine_apply(Callable&& callable, Tuple&& tuple, std::false_type)
        {
            return std::forward<Callable>(callable)(std::forward<Tuple>(tuple));
        }
    }

    template <typename Callable, typename T>
    decltype(auto) genuine_apply(Callable&& callable, T&& argument)
    {
        return detail::genuine_apply(std::forward<Callable>(callable), std::forward<T>(argument),
                                     is_tuple_like<std::decay_t<T>>{});
    };

    template <typename Generator, typename ... Callables>
    class benchmark
    {
        Generator gen;
        std::tuple<Callables...> callables;
        std::vector<std::array<std::chrono::duration<double>, sizeof...(Callables)>> timings;
        std::vector<typename Generator::input_type> inputs;
    public:
        using input_type = typename Generator::input_type;
        static constexpr std::size_t function_count = sizeof...(Callables);

        template <typename Gen,
                  typename = shino::enable_sfinae<Gen, Generator>,
                  typename ... ArgTypes>
        benchmark(Gen&& generator, ArgTypes&& ... args):
                gen(std::forward<Gen>(generator)),
                callables(std::forward_as_tuple(std::forward<ArgTypes>(args)...))
        {}

        template <typename Gen,
                  typename = shino::enable_sfinae<Gen, Generator>,
                  typename Tuple>
        benchmark(Gen&& generator, Tuple&& tup):
                gen(std::forward<Gen>(generator)),
                callables(std::forward<Tuple>(tup))
        {}

        template <typename InputType,
                  typename = enable_sfinae<InputType, input_type>>
        void time(InputType&& input,
                  std::size_t runcount)
        {
            inputs.push_back(input);
            time_all(std::make_index_sequence<sizeof...(Callables)>{},
                     std::forward<InputType>(input), runcount);
        }

        template <typename OutputIterator,
                  typename Unit = std::chrono::milliseconds>
        void get_as(OutputIterator first)
        {
            for (const auto& timing : timings)
            {
                std::array<Unit, function_count> converted_readings;
                std::transform(timing.begin(), timing.end(), converted_readings.begin(),
                               [](const auto& reading) {
                                   return std::chrono::duration_cast<Unit>(reading);
                               });
                *first++ = converted_readings;
            }
        }

        template <typename Unit = std::chrono::milliseconds>
        auto get_as()
        {
            std::vector<std::array<Unit, function_count>> converted_readings(timings.size());

            get_as<Unit>(converted_readings.begin());
            return converted_readings;
        }

        auto timings_begin() const
        {
            return timings.begin();
        }

        auto timings_end() const
        {
            return timings.end();
        }

        auto inputs_begin() const
        {
            return inputs.begin();
        }

        auto inputs_end() const
        {
            return inputs.end();
        }

    private:
        template <std::size_t Index, typename InputType,
                                     typename = enable_sfinae<InputType, input_type>>
        auto time_one(InputType&& input,
                      std::size_t runcount)
        {
            std::chrono::duration<double> timing(0);

            for (std::size_t i = 0; i < runcount; ++i)
            {
                auto callable_input = gen(input); //separate input creation from benchmark
                auto start = std::chrono::high_resolution_clock::now();
                shino::genuine_apply(std::get<Index>(callables), callable_input);
                auto end = std::chrono::high_resolution_clock::now();
                timing += end - start;
            }

            return timing / runcount;
        }

        template <std::size_t ... Indices,
                typename InputType,
                typename = shino::enable_sfinae<InputType, input_type>>
        void time_all(std::index_sequence<Indices...>,
                      InputType&& input,
                      std::size_t runcount)
        {
            std::array<std::chrono::duration<double>, sizeof...(Callables)> a_run =
                    {time_one<Indices>(std::forward<InputType>(input), runcount)...};
            timings.push_back(a_run);
        }
    };

    template <typename Gen, typename ... Callables>
    auto benchmarker(Gen&& generator, Callables&& ... callables)
    {
        return benchmark<std::decay_t<Gen>,
                std::decay_t<Callables>...>{std::forward<Gen>(generator),
                                            std::forward_as_tuple(std::forward<Callables>(callables)...)};
    }
}

#endif //AREA51_BENCHMARK_V2_HPP



int main()
{

}