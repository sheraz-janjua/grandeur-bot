/*
    main.js
    Initialize the SDK and get
    a reference to the project
*/

var project = grandeur.init("grandeurkv7jxe7700af0k178y0f1xcf", "accesskvnwixok03jh0ixfefx1hftc", "eyJ0b2tlbiI6ImV5SmhiR2NpT2lKSVV6STFOaUlzSW5SNWNDSTZJa3BYVkNKOS5leUpwWkNJNkltRmpZMlZ6YzJ0MmJuZHBlRzlyTUROcWFEQnBlR1psWm5neGFHWjBZeUlzSW5SNWNHVWlPaUpoWTJObGMzTWlMQ0pwWVhRaU9qRTJNell5TURnNE9EZDkuZ0ZwTjFIMzhLaDZiaHBPa3FZbGpFdXNSVW1YNVZCSzM5T3NUczE5NlM3VSJ9");

var timer = null;

/* Setting the connection status update handler */
project.onConnection((status) => {
  /* 
      This callback gets fired
      whenever the connection status
      changes
  */

  switch(status) {
    case "CONNECTED": 
        /*
            If SDK is connected,
            we set the status.
        */
        document.getElementById("status").innerText = "Connected";

        /* Here we set up the timer to update parms every 5 seconds */
        timer = setInterval(async function() { 
            /* 
                This function updates the device parameters
                and set the state to a random string.
            */
            
            var deviceID = "devicekvj8xwa60ipx0k17hxk63shm";            
            /* Here we use *Date* for a random state value */
            var f = "*";
            
            /* Gets reference to device class */
            var devices = project.devices();

            /* Updates the device state */
            await devices.device(deviceID).data().set("f", "*");
   
            /* Logs the state to browser's console */  
            console.log(f);
        }, 5000);
        
        break;
    default: 
        /* If SDK gets disconnected, we display the status
           on the app and clear the timer.
         */
        document.getElementById("status").innerText = "Disconnected";

        /* Clears timer */
        clearInterval(timer);
  }
});

/* Function to login user */
async function login() {
    /* Store credentials into variables */
    var email = "user@somemail.com";
    var password = "userpass";

    /* Set the status to logging in */
    document.getElementById("status").innerText = "Logging in";

    /* Then get a reference to auth class */
    var auth = project.auth();

    /* and in a try catch block */
    try {
        /* Submit request using login function */
        var res = await auth.login(email, password);

    /* 
        Got the response to login
        handle response
    */
    switch(res.code) {
      case "AUTH-ACCOUNT-LOGGEDIN": 
      case "AUTH-ACCOUNT-ALREADY-LOGGEDIN":
        /*
            User Authenticated
            Set the status to success
        */
        document.getElementById("status").innerText = "User Authenticated";
        break;

      default: 
        /* 
            Logging failed due
            to invalid data
        */
        document.getElementById("status").innerText = "Authentication Failed";
    }
  }
  catch(err) {
    /*
        Error usually got generated when
        we are not connected to the internet
    */
    document.getElementById("status").innerText = "Network Error";
  }
}

/* Call login on startup */
login();