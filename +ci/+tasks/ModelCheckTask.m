classdef ModelCheckTask < matlab.buildtool.Task
    %MODELCHECKTASK Custom build task for Simulink structural checks.

    properties
        ModelName (1,1) string {mustBeNonmissing}
        ModelFile matlab.buildtool.io.File {mustBeScalarOrEmpty}
    end

    properties (TaskInput)
        TreatWarningsAsErrors (1,1) logical = false
    end

    methods
        function task = ModelCheckTask(modelName, modelFile, options)
            arguments
                modelName (1,1) string {mustBeNonmissing}
                modelFile (1,1) matlab.buildtool.io.File
                options.Description (1,1) string = "Check Simulink model"
                options.Dependencies (1,:) string = string.empty(1,0)
                options.TreatWarningsAsErrors (1,1) logical = false
            end

            task.ModelName = modelName;
            task.ModelFile = modelFile;
            task.Description = options.Description;
            task.Dependencies = options.Dependencies;
            task.TreatWarningsAsErrors = options.TreatWarningsAsErrors;
            task.DisableIncremental = true;
        end
    end

    methods (TaskAction, Sealed, Hidden)
        function checkModel(task, ~)
            modelPath = task.ModelFile.absolutePaths();
            if ~isfile(modelPath)
                error("ci:ModelCheck:ModelMissing", ...
                    "Model file does not exist: %s", modelPath);
            end

            cleanup = onCleanup(@()closeLoadedModel(task.ModelName)); %#ok<NASGU>
            load_system(modelPath);

            assertNoDanglingLines(task.ModelName);

            evalc("set_param(task.ModelName, 'SimulationCommand', 'update')");

            if task.TreatWarningsAsErrors
                warnState = warning("query", "last");
                if strlength(string(warnState.identifier)) > 0
                    error("ci:ModelCheck:WarningAsError", ...
                        "Model check produced warning %s: %s", ...
                        warnState.identifier, warnState.message);
                end
            end

            fprintf("Model check passed for %s.\n", task.ModelName);
        end
    end
end

function assertNoDanglingLines(modelName)
lines = find_system(modelName, FindAll="on", Type="line");
if isempty(lines)
    return
end

srcPorts = get_param(lines, "SrcPortHandle");
dstPorts = get_param(lines, "DstPortHandle");
if ~iscell(srcPorts)
    srcPorts = {srcPorts};
    dstPorts = {dstPorts};
end

isDangling = cellfun(@(src, dst) src == -1 || isempty(dst) || any(dst == -1), ...
    srcPorts, dstPorts);
if any(isDangling)
    error("ci:ModelCheck:DanglingLine", ...
        "Model %s contains %d dangling signal line(s).", ...
        modelName, nnz(isDangling));
end
end

function closeLoadedModel(modelName)
if bdIsLoaded(modelName)
    close_system(modelName, 0);
end
end
