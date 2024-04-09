# SDK integration for retrieving current IP addresses

- min Android SDK 24
- kotlin DLS build
- NDK version 25.1.8937393
- gradle 8.4

- External libraries:
  - retrofit:2.11.0
  - gson:2.11.0
  - interceptor:4.12.0

# Directory

├── src
    ├── main                                     # Source files (alternatively `lib` or `app`)
      └── cpp                                    # Source files for NDK
        └── native-lib.cpp                       # Source files where we have a function for getting the current IP address (ipAddressFromJNI) 
  
      └── java.com.sourcico.cloudonixndk         # Source files for Java Adnroid
        └── api
          └── ApiService.java                    # Source file where we have defined all endpoint routes
        └── model
          └── IpAddress.java                     # Source file where we define the model of request and respond results
        └── App.java                             # Source file of the started  application file, here we have logic on injecting Retrofit for network calls
        └── MainActivity.java                    # Source file of the started  screen. The MainActivity contains the required layout with some logic when the server retrieves some data based on  the current IP address provided by NDK function 
├── test                                         # Automated tests (alternatively `spec` or `tests`)
└── README.md
