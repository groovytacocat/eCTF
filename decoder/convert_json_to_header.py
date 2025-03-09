import json
import os
# Read JSON file
with open("/global.secrets", "r") as f:
    data = json.load(f)

# Extract key (modify if your JSON structure differs)
root_key = bytes.fromhex(data.get("ROOT", []))
iv = bytes.fromhex(data.get("NONCE", []))
salt = bytes.fromhex(data.get("SALT", []))
# Write to C header file
with open("/decoder/inc/global_secrets.h", "w") as f:
    f.write("#ifndef GLOBAL_SECRETS_H\n")
    f.write("#define GLOBAL_SECRETS_H\n\n")
    f.write("#include <stdint.h>\n\n")
    f.write("#define ROOT_KEY (uint8_t[]) { ")
    f.write(", ".join(map(str, root_key)))
    f.write(" }\n\n")
    f.write("#define NONCE (uint8_t[]) { ")
    f.write(", ".join(map(str,iv)))
    f.write(" }\n\n")
    f.write("#define SALT (uint8_t[]) { ")
    f.write(", ".join(map(str,salt)))
    f.write(" }\n\n")
    f.write("#endif // GLOBAL_SECRETS_H\n")
