/*
    main.js
    Initialize the SDK and get
    a reference to the project
*/
var canvas, ctx;
var project = grandeur.init("grandeurkv7jxe7700af0k178y0f1xcf", "accesskvnwixok03jh0ixfefx1hftc", "eyJ0b2tlbiI6ImV5SmhiR2NpT2lKSVV6STFOaUlzSW5SNWNDSTZJa3BYVkNKOS5leUpwWkNJNkltRmpZMlZ6YzJ0MmJuZHBlRzlyTUROcWFEQnBlR1psWm5neGFHWjBZeUlzSW5SNWNHVWlPaUpoWTJObGMzTWlMQ0pwWVhRaU9qRTJNell5TURnNE9EZDkuZ0ZwTjFIMzhLaDZiaHBPa3FZbGpFdXNSVW1YNVZCSzM5T3NUczE5NlM3VSJ9");
var devices;
var deviceID = "devicekvvxu41q00260pxfcat23jum";
var timer = null;
var connected = false;

var gx = 0;
var gy = 0;


const sens = 10;
const radius = 100;

var joy = new JoyStick('joyDiv');

setInterval(function () {
    const x = joy.GetX();
    const y = joy.GetY();
    send_xy(x, y);
}, 100);

/* Setting the connection status update handler */
project.onConnection((status) => {
    /* 
        This callback gets fired
        whenever the connection status
        changes
    */

    switch (status) {
        case "CONNECTED":
            /*
                If SDK is connected,
                we set the status.
            */
            document.getElementById("status").innerText = "Connected";
            devices = project.devices();
            connected = true;
            break;
        default:
            /* If SDK gets disconnected, we display the status
               on the app and clear the timer.
             */
            document.getElementById("status").innerText = "Disconnected";
            connected = false;
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
        switch (res.code) {
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
                if (res.code == "AUTH-ACCOUNT-ALREADY-LOGGEDIN") dispay_joystick();
        }
    }
    catch (err) {
        /*
            Error usually got generated when
            we are not connected to the internet
        */
        document.getElementById("status").innerText = "Network Error";
    }
}
function send_xy(x, y) {
    if (Math.abs(x - gx) > sens || Math.abs(y - gy) > sens) {
        gx = x;
        gy = y;
        // TODO: send data 
        const p_str = x + ',' + y;
        // console.log(p_str + '\n');
        if (devices && connected) {
            devices.device(deviceID).data().set("p", p_str);
            // console.log(':ok\n');
        }
    }
}

/* Call login on startup */
login();