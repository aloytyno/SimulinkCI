# Simulink CI Example

This repository demonstrates a compact Simulink CI workflow based on the shipping MATLAB/Simulink `vdp` example.

The build uses MATLAB `buildtool` and does not require Simulink Test or Simulink Check.

## Layout

- `models/vdp_ci.slx` contains the project-local Simulink model.
- `tests/tVdpModel.m` contains `matlab.unittest` tests that simulate the model.
- `+ci/+tasks` contains custom buildtool tasks for model checking and code generation.
- `.github/workflows/simulink-ci.yml` is a sample GitHub Actions workflow.

## Run Locally

Open MATLAB in the repository root and run:

```matlab
buildtool
```

The default task is `codegen`, which depends on `modelTests`, which depends on `modelCheck`.

To run individual tasks:

```matlab
buildtool modelCheck
buildtool modelTests
buildtool("codegen")
```

Generated code and cache files are written under `build/`.
In GitHub Actions, generated source under `build/codegen/` is committed back to the repository after successful code generation. Build cache files remain ignored.
