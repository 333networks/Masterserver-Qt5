# Building and Running MasterServer in Docker

Running the MasterServer in Docker may be useful on systems which don't have
appropriate build or runtime environments available to run the service 
natively.

## Build

From the project root directory, run:

```sh
docker build -t 333masterserver:latest .
```

This will compile the source and build a Docker image named `333masterserver`,
tagged as `latest`. 

## Setup

To maintain the database and configuration outside the Docker container
runtime, create some directories on the host to hold these files.

1. Create data directory:
   - note: I recommend creating this in a separate location, such as 
     `/var/lib/masterserver`
   1. `/var/lib/masterserver/data`
   2. Assign ownership to UID 1 (this is the non-root UID the process will run
      as): `chown -R 1 /var/lib/masterserver`
2. Create configuration file in 
   `/var/lib/masterserver/data/MasterServer-Settings.ini`
   - Follow configuration instructions in [README.md](README.md) to populate
     the configuration file appropriately.
3. Copy file `SupportedGames.json` to `/var/lib/masterserver/data/`

## Run

After directories and configuration have been created, run the service as a 
non-interactive daemon process:

```sh
docker run --restart always --name masterserver -d \
 -v /var/lib/masterserver/data:/masterserver/data \
 -v masterserver-log:/masterserver/log \
 -p 27900:27900/udp \
 -p 28900:28900/tcp \
 333masterserver:latest
```
