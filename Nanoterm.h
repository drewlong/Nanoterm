/**
 * Nanoterm.h
 *
 * Created on: 04.08.2023
 *
 * Header file for the Nanoterm.cpp file which implements a TinyTerminal library for Arduino.
 * This library provides a simple command line interface over the Serial port, and includes
 * functionality to register commands, arguments, and parse input.
 *
 * Author: Andrew Long (github.com/drewlong)
 * 
 * This library is free software; you can redistribute it and/or modify it under the terms 
 * of the GNU Lesser General Public License as published by the Free Software Foundation; 
 * either version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this 
 * library; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA 02110-1301 USA
 *
 */

#ifndef TERMINAL_H
#define TERMINAL_H

#include <Arduino.h>
#include <map>
#include <string>
#include <vector>

// Declare the function pointer type
typedef void (*CommandPointer)();

// Argument map
extern std::map<std::string, std::string> ARGM;

// Generic Commands
void clearScreen(); // function to clear the terminal screen

// Utils
void buildMap(const std::vector<std::string>& keys, const std::vector<CommandPointer>& functions); // function to build command map from keys and function pointers
void printTerm(); // function to print the prompt
void getInput(); // function to get user input from Serial 
void parseInput(std::string input); // function to parse user input into commands and arguments
String retInput(); // function to get user input as String

// Core functions
void addToCommandMap(std::string key, CommandPointer function); // function to add a command to the command map
void callCommand(std::string cmd); // function to call a command from the command map

#endif /* TERMINAL_H */
