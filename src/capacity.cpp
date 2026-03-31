#include "constrained/capacity.h"
#include "constrained/fsm.h"
#include "constrained/states.h"

#include <cmath>

std::vector<std::vector<double>> buildAdjacencyMatrix(
    const std::vector<std::string>& states,
    int maxRunLength) {
    int n = static_cast<int>(states.size());
    std::vector<std::vector<double>> A(n, std::vector<double>(n, 0.0));

    for (int i = 0; i < n; ++i) {
        for (char bit : {'0', '1'}) {
            std::string nextState = applyBitToState(states[i], bit, maxRunLength);

            if (!nextState.empty()) {
                int j = findStateIndex(states, nextState);
                if (j != -1) {
                    A[i][j] += 1.0;
                }
            }
        }
    }

    return A;
}

double largestEigenvalue(const std::vector<std::vector<double>>& A,
    int maxIterations,
    double tolerance) {
    int n = static_cast<int>(A.size());

    std::vector<double> x(n, 1.0);
    std::vector<double> y(n, 0.0);

    double lambdaOld = 0.0;
    double lambdaNew = 0.0;

    for (int iter = 0; iter < maxIterations; ++iter) {
        for (int i = 0; i < n; ++i) {
            y[i] = 0.0;
            for (int j = 0; j < n; ++j) {
                y[i] += A[i][j] * x[j];
            }
        }

        double norm = 0.0;
        for (double v : y) {
            norm += v * v;
        }
        norm = std::sqrt(norm);

        if (norm == 0.0) {
            return 0.0;
        }

        for (int i = 0; i < n; ++i) {
            x[i] = y[i] / norm;
        }

        lambdaNew = norm;

        if (std::fabs(lambdaNew - lambdaOld) < tolerance) {
            break;
        }

        lambdaOld = lambdaNew;
    }

    return lambdaNew;
}

CapacityResult computeCapacity(const std::vector<std::string>& states,
    int maxRunLength) {
    CapacityResult result;

    std::vector<std::vector<double>> A = buildAdjacencyMatrix(states, maxRunLength);
    result.lambda = largestEigenvalue(A);
    result.capacity = (result.lambda > 0.0) ? std::log2(result.lambda) : 0.0;

    return result;
}