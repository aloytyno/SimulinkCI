classdef CodegenTask < matlab.buildtool.Task
    %CODEGENTASK Custom build task for Simulink Coder code generation.

    properties
        ModelName (1,1) string {mustBeNonmissing}
        ModelFile matlab.buildtool.io.File {mustBeScalarOrEmpty}
    end

    properties (TaskOutput)
        CodegenFolder (1,1) matlab.buildtool.io.File = "build/codegen"
        CacheFolder (1,1) matlab.buildtool.io.File = "build/cache"
    end

    methods
        function task = CodegenTask(modelName, modelFile, options)
            arguments
                modelName (1,1) string {mustBeNonmissing}
                modelFile (1,1) matlab.buildtool.io.File
                options.Description (1,1) string = "Generate code for Simulink model"
                options.Dependencies (1,:) string = string.empty(1,0)
                options.CodegenFolder (1,1) matlab.buildtool.io.File = "build/codegen"
                options.CacheFolder (1,1) matlab.buildtool.io.File = "build/cache"
            end

            task.ModelName = modelName;
            task.ModelFile = modelFile;
            task.Description = options.Description;
            task.Dependencies = options.Dependencies;
            task.CodegenFolder = options.CodegenFolder;
            task.CacheFolder = options.CacheFolder;
            task.DisableIncremental = true;
        end
    end

    methods (TaskAction, Sealed, Hidden)
        function generateCode(task, ~)
            modelPath = task.ModelFile.absolutePaths();
            if ~isfile(modelPath)
                error("ci:Codegen:ModelMissing", ...
                    "Model file does not exist: %s", modelPath);
            end

            codegenFolder = task.CodegenFolder.absolutePaths();
            cacheFolder = task.CacheFolder.absolutePaths();
            ensureFolder(codegenFolder);
            ensureFolder(cacheFolder);

            oldCodegenFolder = Simulink.fileGenControl("get", "CodeGenFolder");
            oldCacheFolder = Simulink.fileGenControl("get", "CacheFolder");
            cleanup = onCleanup(@()restoreFileGenAndClose( ...
                oldCodegenFolder, oldCacheFolder, task.ModelName)); %#ok<NASGU>

            Simulink.fileGenControl("set", ...
                CodeGenFolder=codegenFolder, ...
                CacheFolder=cacheFolder, ...
                CreateDir=true);

            load_system(modelPath);
            slbuild(task.ModelName);

            fprintf("Code generation completed for %s.\n", task.ModelName);
        end
    end
end

function ensureFolder(folder)
if ~isfolder(folder)
    mkdir(folder);
end
end

function restoreFileGenAndClose(codegenFolder, cacheFolder, modelName)
Simulink.fileGenControl("set", ...
    CodeGenFolder=codegenFolder, ...
    CacheFolder=cacheFolder, ...
    CreateDir=true);
if bdIsLoaded(modelName)
    close_system(modelName, 0);
end
end
