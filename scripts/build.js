#!/usr/bin/env node

//'use strict';

let fs = require('fs')

let start_dir = process.cwd();

var args = []
var options = []
var file

const { spawnSync } = require("child_process")

var engine = false
var verbose = false
var dir = process.cwd()

function print(str) { console.log(str) }

function printe(str) { console.error('\x1b[31m%s\x1b[0m', str) }

function printw(str) { console.warn('\x1b[33m%s\x1b[0m', str) }

function printv(str) { verbose ? console.log(str) : {} }

process.argv.forEach(function (val, index, array) {
    //console.log(index + ': ' + val)
    if (index > 1) {
        args.push(val)
    }
})

if (args.length > 0) {
    args.some(function (val) {
        if (val.substr(0, 2) == "--") {
            options.push(val.substr(2, val.length - 2))
        } else if (val.substr(0, 1) == "-") {
            options.push(val.substr(1, val.length - 1))
        }
        else {
            file = val
        }
    })
}

if (file == null) {
    printe("No Project Config Specified")
    process.exit()
}

if (options.length != 0) {
    options.some(function (opt) {
        if (opt.includes("=")) {
            var split = opt.split("=")

            if (split.length > 2 || split.length < 2 || split[1] == null) {
                printe("Error parsing argument '" + opt + "'")
                process.exit()
            }
            switch (split[0]) {
                case "dir":
                case "directory":
                case "d":
                    dir = split[1]
                    break
            }
        }
        else {

            switch (opt) {
                case "engine":
                case "e":
                    engine = true
                    break
                case "v":
                case "verbose":
                    verbose = true
                    break
            }
        }
    })
}


var path = dir + "/" + file
var buffer = fs.readFileSync(path)

var config

try {
    config = JSON.parse(buffer.toString())
} catch (e) {
    if (e instanceof SyntaxError) {
        printe("Error parsing config: Invalid JSON")
        process.exit()
    } else {
        printe(e.message)
    }
}

engine = config["engine"] || engine;

function mkdir(name) { try { fs.mkdirSync(name) } catch (err) { if (err.code !== 'EEXIST') throw err } }

function cd(dir) { process.chdir(dir) }

mkdir("working")

mkdir("working/cmake")

mkdir("working/ckb")

cd("working")

function exec(command, args = [], stdout = true, stderr = true) {
    var result = spawnSync(command, args)
    if (result.stderr.toString() && stderr) {
        printe(result.stderr.toString())
    }
    if (result.stdout.toString() && stdout) {
        print(result.stdout.toString())
    }
}
var src = config["src"];
if (src == null) {
    src = "../../"
} else {
    src = "../../" + src
}

function findEngine() {
    if (engine) {
        var out = {
            headers: "",
            lib: "",
            libLoc: ""
        }

        return out
    }
    var out = {}

    if (process.platform == 'win32') {
        printe("Windows support needs to be implemented")
        process.exit(-1)
    }

    if (!fs.exists("/opt/CodekraftEngine")) {
        printe("Engine is not in /opt/CodekraftEngine, please install it")
        process.exit(-1)
    }

    out.headers = "/opt/CodekraftEngine/include/"
    out.libLoc = "/opt/CodekraftEngine/lib"
    out.lib = "CodekraftEngine"

    return out
}

var cmakeFile = "ERROR OPENING"

var ef = findEngine();

var NAME = config["name"]
var ENGINE_HEADERS = ef.headers
var ENGINE_LIB = ef.lib
var ENGINE_LIB_LOC = ef.libLoc
var SRC = config["src"]
var PREC = config["preCompile"]
var CLINK
try {
    CLINK = config["linkCode"]
} catch (e) {
    CLINK = `target_link_libraries(${NAME} ${ENGINE_LIB})`
}
var CFIND = config["find"]
cmakeFile = `
cmake_minimum_required(VERSION 3.5)
project(${NAME})

cmake_policy(SET CMP0072 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

set(NAME ${NAME})

${FIND}

set(CMAKE_BUILD_TYPE Debug)

file(GLOB SRC "${SRC}/*.cpp")

${PREC}

include_directories(${ENGINE_HEADERS})
link_directories(${ENGINE_LIB_LOC})

add_executable(${NAME} \${SRC})
#target_link_libraries(${NAME} \${CMAKE_BINARY_DIR}/engine/libCKEngine.so)
${LINK}
`
function runCmake() {
    exec("cmake", ["..", "-GNinja"])
    let f_dir = process.cwd();
    //cd(start_dir)
    cd("cmake")
    if (engine) {
        print(process.cwd())
        var ckl = "../../" + config["cmake"]
        exec("cmake", [ckl, "-GNinja"])
    }
    else {
        exec("cmake", ["..", "-GNinja"])
    }

    cd(f_dir)
}
print(start_dir)


// First cmake to generate compile commands
exec("cmake", ["..", "-GNinja"])


// Run CodekraftBuild tool
cd(start_dir)
cd("working")
cd("ckb")
exec("ckbuild", [start_dir + "/working/cmake/compile_commands.json"])
// Run CMake again
exec("cmake", ["..", "-GNinja"])

// Run Ninja
cd(start_dir)
cd("working")
cd("cmake")
exec("ninja", [])