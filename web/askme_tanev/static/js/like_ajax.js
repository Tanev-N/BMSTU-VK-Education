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


function init(item_name) {
    console.log("lol");
    let cards;
    if (item_name === 'Question')
    {
        cards = document.querySelectorAll('.question');
    }
    if (item_name === 'Answer')
    {
        cards = document.querySelectorAll('.answer');
    }

    for(const card of cards) {
        console.log('giper_imba');
        const likeButton = card.querySelector('.like-button');
        const likeCounter = card.querySelector('.like-counter');
        const dislikeButton = card.querySelector('.dislike-button');
        let ItemId;
        if (item_name === 'Question') {
            console.log('imba');
            ItemId = card.dataset.questionid;
        }
        if (item_name === 'Answer') {
            console.log('neimba2');
            ItemId = card.dataset.answerid;
        }
        console.log(ItemId);



        likeButton.addEventListener('click', () => {
            const request = new Request(`/like_ajax/${ItemId}`, {method: 'post', headers: {
                'Content-Type': 'application/json',
                'X-CSRFToken': getCookie('csrftoken')
                }, body: JSON.stringify( {
                    like: 'True',
                    item: `${item_name}`
                    }
                )});
            fetch(request)
                .then((response) => response.json())
                .then((data) => likeCounter.innerHTML = data.likes_count);
        } )

        dislikeButton.addEventListener('click', () => {
            const request = new Request(`/like_ajax/${ItemId}`, {method: 'post', headers: {
                'Content-Type': 'application/json',
                'X-CSRFToken': getCookie('csrftoken')
                }, body: JSON.stringify( {
                    like: 'False',
                    item: `${item_name}`
                    }
                )});
            fetch(request)
                .then((response) => response.json())
                .then((data) => likeCounter.innerHTML = data.likes_count);
        })

    }
}
