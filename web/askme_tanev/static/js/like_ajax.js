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
        const likeImage = card.querySelector('.like-image');
        const dislikeImage = card.querySelector('.dislike-image');
        let ItemId;
        if (item_name === 'Question') {
            ItemId = card.dataset.questionid;
        }
        if (item_name === 'Answer') {
            ItemId = card.dataset.answerid;
        }
        console.log(ItemId);





        const request = new Request(`/check_like_ajax/${ItemId}`, {method: 'post', headers: {
                'Content-Type': 'application/json',
                'X-CSRFToken': getCookie('csrftoken')
                }, body: JSON.stringify( {
                    like: 'True',
                    item: `${item_name}`
                    }
                )});
            fetch(request)
                .then((response) => response.json())
                .then((data) => {

                    if (data.liked) {
                        likeImage.src = '/static/img/icons/like-fill.svg';
                    }
                    else {
                        likeImage.src = '/static/img/icons/like.svg';
                    }


        });
            const request2 = new Request(`/check_like_ajax/${ItemId}`, {method: 'post', headers: {
                'Content-Type': 'application/json',
                'X-CSRFToken': getCookie('csrftoken')
                }, body: JSON.stringify( {
                    like: 'False',
                    item: `${item_name}`
                    }
                )});
            fetch(request2)
                .then((response) => response.json())
                .then((data) => {

                    if (data.liked) {
                        dislikeImage.src = '/static/img/icons/dislike-fill.svg';
                    }
                    else {
                        dislikeImage.src = '/static/img/icons/dislike.svg';
                    }


        });




       

       

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
                .then((data) =>  {
                    likeCounter.innerHTML = data.likes_count;

                    if (data.liked) {
                        likeImage.src = '/static/img/icons/like-fill.svg'
                        dislikeImage.src = '/static/img/icons/dislike.svg';
                    }
                    else {
                        likeImage.src = '/static/img/icons/like.svg';
                    }

                });
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
                .then((data) => {
                    likeCounter.innerHTML = data.likes_count;

                    if (data.liked) {
                        likeImage.src = '/static/img/icons/like.svg'
                        dislikeImage.src = '/static/img/icons/dislike-fill.svg';
                    }
                    else {
                        dislikeImage.src = '/static/img/icons/dislike.svg';
                    }

                });
        })

    }
}
