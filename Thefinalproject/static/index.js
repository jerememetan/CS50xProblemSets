
document.addEventListener('DOMContentLoaded', function() {

    let id = document.querySelectorAll("#delete_button");
    let deleteButton = document.querySelectorAll("#delete_id");

    for (let i = 0; i < id.length; i++) {
        let number = id[i].getAttribute("value");
        deleteButton[i].value = number;
    }
    // TODO: Implement seperating and joining of bookings
    let localStorage = window.sessionStorage;
    let seperated_btn = document.getElementById("seperated_bookings_btn");
    let joint_btn = document.getElementById("joint_bookings_btn");
    // first time user will have a seperated bookings selected

    function loadtable() {

        console.log("before if statements" + localStorage.getItem("sorted_bookings_btn"))
        if (localStorage.getItem("sorted_bookings_btn") == "seperated_bookings_btn") {
            // REMOVE THE JOINT BOOKINGS, DISPLAY ONLY SEPERATED BOOKINGS
            seperated_btn.checked = true;
            document.getElementById("joint_table").hidden = true;
            document.getElementById("seperated_table").hidden = false;
            console.log("sorted_bookings_btn" + localStorage.getItem("sorted_bookings_btn"))
            return 1;
        } else if (localStorage.getItem("sorted_bookings_btn") == "joint_bookings_btn") {
            // REMOVE THE SEPERATED BOOKINGS, DISPLAY ONLY JOINT BOOKINGS
            joint_btn.checked = true;
            document.getElementById("joint_table").hidden = false;
            document.getElementById("seperated_table").hidden = true;
            console.log("sorted_bookings_btn" + localStorage.getItem("sorted_bookings_btn"))
            return 1;
        } else {
            document.getElementById("joint_table").hidden = true;
            document.getElementById("seperated_table").hidden = false;
            localStorage.setItem("sorted_bookings_btn", "seperated_bookings_btn");
            console.log("sorted_bookings_btn" + localStorage.getItem("sorted_bookings_btn"))
            return 0; // 0 = error/no input field
        }
    }
    loadtable();
    // add event listeners for both checkboxes
    seperated_btn.addEventListener('change', function(){
        if (this.checked)
        {
            localStorage.setItem("sorted_bookings_btn", "seperated_bookings_btn");
            console.log("button is clicked!");
            loadtable();

        }
    });

    joint_btn.addEventListener('change', function(){
        if (this.checked)
        {
            localStorage.setItem("sorted_bookings_btn", "joint_bookings_btn");
            console.log("button is clicked!");
            loadtable();
        }
    });



    const modal = document.getElementById("Modal")
    if (modal) {
        console.log("modal:",modal)
        modal.addEventListener('shown.bs.modal', event => {
            console.log("inside of modal test, event:", event, modal);

            let button = event.relatedTarget;

            console.log("inside of modal test", button);
            // define values on button's attributes
            let date_str = button.getAttribute('date');
            let date_parts = date_str.split("/");
            // format date
            let date = date_parts[2] + "-" + date_parts[1] + "-" + date_parts[0];
            console.log("date:" + date);
            let start_time = button.getAttribute('start_time');
            let end_time = button.getAttribute('end_time');
            let contact = button.getAttribute('contact');
            let type = button.getAttribute('detail');
            // splits types by commas
            let type_parts = type.split(","); // ["addinfo", "Lunch"]
            console.log("type parts:" + type_parts[0]);
            let buttonid = button.getAttribute('id');
            // modal's query selectors
            let dateinput = modal.querySelector('.date');
            let detail = modal.querySelector('#details');
            let number = modal.querySelector('#number');
            console.log("detail:", detail)
            // pre setting the modal value accordingly
            dateinput.value = date;
            detail.value = type;
            number.value = contact;
            console.log("detail value:", type)

            // pre setting the time input accordingly
            document.getElementById('booking_id').value = buttonid;
            let selectedStartOption = document.getElementById('start-'+ start_time);
            selectedStartOption.selected = true;

            let selectedEndOption = document.getElementById('end-'+ end_time);
            selectedEndOption.selected = true;
            let tagElements = modal.querySelectorAll('[name^="tag"]');
            console.log("tagElements", tagElements)

            for (let j = 0; j < tagElements.length; j++) {
                for (let k = 0; k < type_parts.length; k++) {
                    if (tagElements[j].id === type_parts[k].trim()) {
                        tagElements[j].checked = true;
                        break;
                    } else {
                        tagElements[j].checked = false;
                    }
                }
                console.log("looping thru tagElement", tagElements[j])
                tagElements[j].addEventListener('click', function() {
                    // if checked, append into input field
                    if (this.checked) {
                        let regex = new RegExp("\\s*" + this.id.trim() + "(?:\\s*,)?\\s*","g");
                        if (detail.value.trim() === "")
                        {
                            detail.value = this.id;
                        }
                        else
                        {
                            detail.value += ", " + this.id;
                        }
                        console.log("appending detail", this)
                    }
                    // if unchecked, remove ONLY the word from the input field
                    if (!this.checked) {
                        let regex = new RegExp("\\s*" + this.id.trim() + "\\b(?:\\s*,)?\\s*","g");
                        detail.value = detail.value.replace(regex, "");
                        // remove trailing comma if exists
                        if (detail.value.endsWith(",")) {
                            detail.value = detail.value.slice(0, -1);
                        }
                    }
                    if (detail.value.trim() === "") {
                        detail.value = detail.value.trim();
                    }
                });
            }
        });
    }

});
