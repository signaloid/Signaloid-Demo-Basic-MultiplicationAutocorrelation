[<img src="https://assets.signaloid.io/add-to-signaloid-cloud-logo-dark-v6.png#gh-dark-mode-only" alt="[Add to signaloid.io]" height="30">](https://signaloid.io/repositories?connect=https://github.com/signaloid/Signaloid-Demo-Basic-MultiplicationAutocorrelation#gh-dark-mode-only)
[<img src="https://assets.signaloid.io/add-to-signaloid-cloud-logo-light-v6.png#gh-light-mode-only" alt="[Add to signaloid.io]" height="30">](https://signaloid.io/repositories?connect=https://github.com/signaloid/Signaloid-Demo-Basic-MultiplicationAutocorrelation#gh-light-mode-only)

# MICRO Benchmark: `double-multiply-autocorrelation`

Benchmark from Tsoutsouras et al. MICRO paper.[^0]

The benchmark reads samples from a file, loads them on a distributional variable and then performs multiplication of the variable with itself. The product of the multiplication is a distributional variable.

## Arguments

```
double-multiply-autocorrelation -a <samples file> -m <mode>
	-a <samples file>: set to `input-A.txt` by default
	-m <mode>: 1 for explicit computation, 0 for implicit uncertainty tracking (0 is the default)
```

## Inputs

The samples are stored in a text file.
The first line of the file is the number of samples that follow.

## Outputs

When running the implicit uncertainty tracking, the result may not be what you expect.
Do not forget to check the underlying distribution.

With the implicit uncertainty tracking, the variable `A` gets a floating point value and an associated distribution.
What gets printed is the value squared.
The distribution squared is displayed as an histogram.

[^0]: Vasileios Tsoutsouras, Orestis Kaparounakis, Bilgesu Arif Bilgin, Chatura Samarakoon, James Timothy Meech, Jan Heck, Phillip Stanley-Marbell: The Laplace Microarchitecture for Tracking Data Uncertainty and Its Implementation in a RISC-V Processor. MICRO 2021: 1254-1269
