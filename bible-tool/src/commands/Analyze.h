#pragma once

#include "Cmd.h"

/// command usage example 
/// --analyze --method=word --src-file=res\file_name --word=word_to_search
/// result will be saved to folder 'output'
class Analyze : public Cmd {
public:
    Analyze(const wstring_t& cmdName);

    virtual void initialize(wstring_t& cmdArgs);
    virtual void doCommand();
    virtual void showHelp();
};