//
// Copyright (C) 2020 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

//! Command to control profcollectd behaviour.

use anyhow::{Context, Result};
use clap::{Args, Parser, Subcommand};

#[derive(Parser)]
#[command(about = "Command interface for profcollectd", long_about = None)]
struct Cli {
    #[command(subcommand)]
    command: Commands,
}

#[derive(Subcommand)]
enum Commands {
    /// Request an one-off system-wide trace.
    Trace(TraceArgs),
    /// Convert traces to perf profiles.
    Process,
    /// Create a report containing all profiles.
    Report,
    /// Clear all local data and reset the state.
    Reset,
}

#[derive(Args)]
struct TraceArgs {
    #[arg(short = 't', long = "tag", default_value_t = String::from("manual"))]
    tag: String,
    #[arg(short = 'd', long = "duration", default_value_t = 1000)]
    duration_ms: i32,
}

fn main() -> Result<()> {
    libprofcollectd::init_logging();

    let cli = Cli::parse();
    match &cli.command {
        Commands::Trace(TraceArgs { tag, duration_ms }) => {
            println!("Performing system-wide trace");
            libprofcollectd::trace_system(tag, *duration_ms).context("Failed to trace.")?;
        }
        Commands::Process => {
            println!("Processing traces");
            libprofcollectd::process().context("Failed to process traces.")?;
        }
        Commands::Report => {
            println!("Creating profile report");
            let path = libprofcollectd::report().context("Failed to create profile report.")?;
            println!("Report created at: {}", &path);
        }
        Commands::Reset => {
            libprofcollectd::reset().context("Failed to reset.")?;
            println!("Reset done.");
        }
    }
    Ok(())
}
