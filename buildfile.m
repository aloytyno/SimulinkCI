function plan = buildfile
%BUILDFILE Build plan for the Simulink CI example.

import ci.tasks.CodegenTask
import ci.tasks.ModelCheckTask
import matlab.buildtool.tasks.TestTask

modelName = "vdp_ci";
modelFile = fullfile("models", modelName + ".slx");

plan = buildplan;

plan("modelCheck") = ModelCheckTask(modelName, modelFile, ...
    Description="Check the Simulink model for structural and compile issues");

plan("modelTests") = TestTask("tests", ...
    Dependencies="modelCheck", ...
    Description="Run MATLAB unit tests that exercise the Simulink model");

plan("codegen") = CodegenTask(modelName, modelFile, ...
    Dependencies="modelTests", ...
    Description="Generate and compile code for the Simulink model");

plan.DefaultTasks = "codegen";
end
