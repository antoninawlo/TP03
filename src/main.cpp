#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <matplot/matplot.h>
#include <vector>
#include <iostream>
#include <math.h>
#include <conio.h>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;
namespace plt = matplot;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void simple_plot();
int add(int, int);

void plot_signal(const std::vector<double>&, double, const std::string&);
std::vector<double> generate_signal(double , double , double , const std::string& );

int main(){
    double freq, sampl_rate, durat;
    std::string signal_type, signal_name;
    std::cout << "Podaj wlasciwosci sygnalu: \n";
    std::cout << "czestotliwosc: ";
    std::cin >> freq;
    std::cout << "\n czestotliwosc probkowania: ";
    std::cin >> sampl_rate;
    std::cout << "\n dlugosc: ";
    std::cin >> durat;
    std::cout << "\n typ sygnalu: ";
    std::cin >> signal_type;
    std::cout << "\n jak chcesz nazwac ten sygnal?";
    std::cin >> signal_name;

    std::vector<double> sygnal = generate_signal(freq, sampl_rate, durat, signal_type);
    plot_signal(sygnal, sampl_rate, signal_name);
    return 0;
}

int add(int i, int j) {
    return i + j;
}

void simple_plot() {
    std::vector<double> x = {1, 2, 3, 4, 5};
    std::vector<double> y = {1, 4, 9, 16, 25};

    plt::plot(x, y);
    plt::show();
}

std::vector<double> generate_signal(double freq, double sampling_rate, double duration, const std::string& signal_type) {
    int N = static_cast<int>(sampling_rate * duration);
    std::vector<double> signal(N);
    for (int i = 0; i < N; ++i) {
        double t = i / sampling_rate;
        if (signal_type == "sin") {
            signal[i] = sin(2 * M_PI * freq * t);
        } else if (signal_type == "cos") {
            signal[i] = cos(2 * M_PI * freq * t);
        } else if (signal_type == "square") {
            signal[i] = (sin(2 * M_PI * freq * t) >= 0 ? 1.0 : -1.0);
        } else if (signal_type == "sawtooth") {
            signal[i] = 2 * (t * freq - floor(t * freq + 0.5));
        } else {
            throw std::invalid_argument("Unsupported signal type");
        }
    }
    return signal;
}

void plot_signal(const std::vector<double>& signal, double sampling_rate, const std::string& title) {
    std::vector<double> time(signal.size());
    for (size_t i = 0; i < signal.size(); ++i) {
        time[i] = i / sampling_rate;
    }

    //plt::figure(figsize=(1200, 780));
    plt::plot(time, signal);
    plt::title(title);
    plt::xlabel("Time [s]");
    plt::ylabel("Amplitude");
    plt::grid(true);
    plt::show();
}

PYBIND11_MODULE(cmake_ex, m) {

    m.def("add", &add, R"pbdoc(
        Add two numbers

    )pbdoc");

    m.def("plot", &simple_plot, "A function that creates a simple plot");

    m.def("main", &main, "A main function");

    m.def("subtract", [](int i, int j) { return i - j; }, R"pbdoc(
        Subtract two numbers

    )pbdoc");
}