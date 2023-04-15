/**
 * Nanoterm.cpp
 *
 * Created on: 04.08.2023
 *
 * Copyright (c) 2023 Andrew Long (github.com/drewlong). All rights reserved.
 * This file is part of the TinyTerminal library for Arduino.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "Nanoterm.h"

String inputString = "";
std::map < std::string, CommandPointer > commandMap;
// Argument map
std::map < std::string, std::string > ARGM;

bool startup = true;
int ci = 0; // command array index
std::string commands[128];

// Input Utils

/**
 * Reads serial input and parses the command and any optional arguments passed.
 * If a command is found, the corresponding function is executed.
 */

void getInput() {
  if (startup) {
    // Wait for serial input on the first loop iteration
    while (Serial.available() == 0) {}
    Serial.print("Terminal started.\n");
    Serial.flush();
    int chipID = ESP.getChipId();
    Serial.printf("ChipID: %d", chipID);
    startup = false;
    printTerm();
  }

  // Read serial input character by character
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == 10 || c == 13) {  // Newline
      if (inputString.length() > 1) {
        std::string cmd = std::string(inputString.c_str());
        parseInput(cmd);
        inputString = "";  // Reset the input string
      }
    } else {
      Serial.write(c);  // Echo the character
      inputString += c;  // Add the character to the input string
    }
  }
}

/**
 * Parses the input string to extract the command and any optional arguments.
 * If a command is found, it is passed to callCommand() for execution.
 * @param input the input string from the serial terminal
 */
void parseInput(std::string input) {
  std::string param;
  std::string last;
  std::string cmd;

  bool c = true;  // Flag to check if first string is the command

  for (auto i = 0u; i < input.length(); i++) {
    if (input[i] == 32) {
      // Check if this is the first string (cmd), set flag after
      if (c) {
        cmd = param;
        c = false;
      }
      // Check for command flags
      if (last[0] == 45) {
        ARGM[last.c_str()] = param.c_str();
      }
      last = param;
      param = "";
    } else {
      param += input[i];
    }
  }
  if (c) {
    callCommand(input);
  } else {
    callCommand(cmd);
  }
}

/**
 * Reads serial input and returns the first newline-terminated string encountered.
 * @return the input string
 */

String retInput() {
  String cmd;
  bool newLine = false;
  String inputString = "";
  
  // Wait for user input
  while (Serial.available() == 0) {
    ;
  }
  
  // Keep reading until a newline character is detected
  while (Serial.available() > 0 || newLine == false) {
    char c = Serial.read();
    if (c == 10) { // newline
      newLine = true;
      if (inputString.length() > 1) {
        cmd = inputString;
      }
    } else if (c > 31 && c < 127) {
      newLine = false;
      Serial.write(c); // echo the character
      inputString += c; // add the character to the input string
    }
  }
  
  return cmd;
}


// Clear the terminal screen by printing a newline character 100 times.
void clearScreen() {
  for (int i = 0; i < 100; i++) {
    Serial.print("\n");
  }
}

// Print the terminal prompt.
void printTerm() {
  Serial.print("\n~#: ");
}

// Build the command map by adding the built-in "clear" command and any user-defined commands.
void buildMap(const std::vector < std::string > & keys,
  const std::vector < CommandPointer > & functions) {

  // Add the built-in "clear" command to the command map.
  commandMap["clear"] = clearScreen;

  // Check that the number of keys and functions match, and print an error message and return if they don't.
  if (keys.size() != functions.size()) {
    Serial.println("ERROR: key vector and function vector must be same size.");
    return;
  }

  // Add each user-defined command to the command map.
  for (size_t i = 0; i < keys.size(); i++) {
    addToCommandMap(keys[i], functions[i]);
  }
}

// Core functions

/**
 * Adds a new command to the command map
 * 
 * @param key      The command string used to call the function
 * @param function A function pointer to the function to be executed
 */
void addToCommandMap(std::string key, CommandPointer function) {
  commandMap[key] = function;
}

/**
 * Calls the function associated with a command string.
 * 
 * @param cmd The command string to execute
 */
void callCommand(std::string cmd) {
  CommandPointer function = commandMap[cmd];
  if (function) {
    function();
    ARGM.clear(); // Clear the argument map
    commands[ci] = cmd; // Add command to history
    ci++;
    printTerm(); // Print the prompt after the command has been executed
  } else {
    Serial.print("\nERROR: Command not found: ");
    Serial.println(cmd.c_str());
    printTerm(); // Print the prompt after the error message
  }
}
