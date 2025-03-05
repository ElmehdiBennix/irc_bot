# IRC Bot

## Overview

This is a simple IRC bot written in C++98 trying to mimic a discord bot, designed to interact with IRC servers. The bot can process commands, handle messages, and perform various automated tasks.

## Features

- Connects to an IRC server.
- Parses and handles IRC messages.
- Supports various commands.
- Implements basic error handling for IRC error codes.

## Installation

### Prerequisites

Make sure you have the following installed:

- A C++98-compatible compiler (e.g., `g++`)
- `make`
- An IRC server for testing (e.g., UnrealIRCd, InspIRCd) or just use already hosted ones

### Build Instructions

Clone the repository and build the bot:

```sh
git clone https://github.com/ElmehdiBennix/irc_bot.git
cd irc_bot
make
```

## Usage

Run the compiled bot:

```sh
./ircBot
```

## Configuration

Modify the configuration settings in the source code to match your IRC server settings (hostname, nickname).

## Contributing

Feel free to fork the repository, submit pull requests, or report issues.
