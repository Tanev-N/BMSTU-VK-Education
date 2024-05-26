function getCookie(name) {
    let cookieValue = null;
    if (document.cookie && document.cookie !== '') {
        const cookies = document.cookie.split(';');
        for (let i = 0; i < cookies.length; i++) {
            const cookie = cookies[i].trim();
            // Does this cookie string begin with the name we want?
            if (cookie.substring(0, name.length + 1) === (name + '=')) {
                cookieValue = decodeURIComponent(cookie.substring(name.length + 1));
                break;
            }
        }
    }
    return cookieValue;
}

function flag_ajax() {

    let answers = document.querySelectorAll('.answer');

    for (const answer of answers) {
        const flag = answer.querySelector('.form-check-input');

        let ItemId = answer.dataset.answerid;

        flag.addEventListener('change', () => {
            const request = new Request(`/answer_correct_flag_ajax/${ItemId}`, {
                method: 'post', headers: {
                    'Content-Type': 'application/json',
                    'X-CSRFToken': getCookie('csrftoken')
                }
            });
            fetch(request)
                .then((response) => response.json())
        })
    }
}