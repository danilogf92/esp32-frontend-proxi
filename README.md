# Proxi ESP32

| Target | ESP32 |
| ----------------- | ----- |

## Step 1

```text
# Build
idf.py set-target esp32
clear
idf.py build
```

## Step 2

```text
# Upload project
idf.py -p COM4 flash
```

## Step 3

```text
# Create proxi server
npx http-server ./web -P http://192.168.4.1
```

## References

- [Official ESP-IDF Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/)
- [C++ Official Documentation](https://en.cppreference.com/w/)
- Tutorials and examples from the ESP32 community
- Additional resources on C++ and embedded programming
