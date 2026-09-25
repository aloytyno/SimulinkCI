classdef tVdpModel < matlab.unittest.TestCase
    %TVDPMODEL Unit tests for the project-local VDP Simulink model.

    properties (Constant)
        ModelName = "vdp_ci"
        ModelFile = fullfile("models", "vdp_ci.slx")
    end

    methods (TestMethodSetup)
        function loadModel(testCase)
            modelFile = testCase.projectModelFile();
            testCase.assertTrue(isfile(modelFile), ...
                "Expected model file to exist.");
            load_system(modelFile);
        end
    end

    methods (TestMethodTeardown)
        function closeModel(testCase)
            if bdIsLoaded(testCase.ModelName)
                close_system(testCase.ModelName, 0);
            end
        end
    end

    methods (Test)
        function modelUsesCodeGenerationSettings(testCase)
            testCase.verifyEqual(string(get_param(testCase.ModelName, "SolverType")), "Fixed-step");
            testCase.verifyEqual(string(get_param(testCase.ModelName, "Solver")), "ode4");
            testCase.verifyEqual(string(get_param(testCase.ModelName, "SystemTargetFile")), "grt.tlc");
        end

        function modelSimulatesToRequestedStopTime(testCase)
            simOut = simulateModel(testCase.ModelName, "2");
            tout = simOut.tout;

            testCase.verifyGreaterThan(numel(tout), 1);
            testCase.verifyEqual(tout(1), 0, AbsTol=eps);
            testCase.verifyEqual(tout(end), 2, AbsTol=1e-12);
        end

        function modelStatesRemainFinite(testCase)
            simOut = simulateModel(testCase.ModelName, "5");
            states = simOut.xout;

            if isa(states, "Simulink.SimulationData.Dataset")
                values = datasetValues(states);
            else
                values = states.signals.values;
            end

            testCase.verifyNotEmpty(values);
            testCase.verifyTrue(all(isfinite(values), "all"), ...
                "Simulation states must remain finite.");
        end
    end

    methods (Access = private)
        function modelFile = projectModelFile(testCase)
            project = currentProject;
            modelFile = fullfile(project.RootFolder, testCase.ModelFile);
        end
    end
end

function simOut = simulateModel(modelName, stopTime)
simIn = Simulink.SimulationInput(modelName);
simIn = simIn.setModelParameter( ...
    StopTime=stopTime, ...
    SaveTime="on", ...
    TimeSaveName="tout", ...
    SaveState="on", ...
    StateSaveName="xout", ...
    ReturnWorkspaceOutputs="on");
simOut = sim(simIn);
end

function values = datasetValues(dataset)
values = [];
for idx = 1:dataset.numElements
    element = dataset.get(idx);
    values = [values; element.Values.Data(:)]; %#ok<AGROW>
end
end
