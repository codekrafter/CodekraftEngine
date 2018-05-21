#!/usr/bin/env node

//'use strict';

let fs = require('fs')

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

print(config["name"])

function mkdir(name) { try { fs.mkdirSync(name) } catch (err) { if (err.code !== 'EEXIST') throw err } }

function cd(dir) { process.chdir(dir) }

mkdir("generated")

mkdir("generated/cmake")

mkdir("generated/ckb")

cd("generated")

cd("cmake")

function exec(command, args) {
    var result = spawnSync(command,args)
    if (result.stderr.toString()) {
        printe(result.stderr.toString())
    }
    if (result.stdout.toString()) {
        print(result.stdout.toString())
    }
}
var src = config["src"];
if(src == null)
{
    src = "../../"
} else
{
    src = "../../" + src
}

let cmakeFile = `
cmake_minimum_required(VERSION 3.5)
project(${NAME})

cmake_policy(SET CMP0072 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE Debug)

file(GLOB SRC "${SRC}/*.cpp")

include_directories(${ENGINE_HEADERS})

add_executable(%${NAME} \${SRC})
target_link_libraries(T${NAME} \${CMAKE_BINARY_DIR}/engine/libCKEngine.so)
`

exec("cmake", [src,"-GNinja"])