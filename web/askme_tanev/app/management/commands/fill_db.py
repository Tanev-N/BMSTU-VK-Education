import random
from django.contrib.auth.models import User
from django.core.management import BaseCommand
from django.db import transaction
from app.models import Profile, Tag, Question, Answer, QuestionLike, AnswerLike


class Command(BaseCommand):

    def add_arguments(self, parser):
        parser.add_argument('ratio', type=int)

    def handle(self, *args, **options):
        ratio = options['ratio']
        num_users = ratio
        num_questions = ratio * 10
        num_answers = ratio * 100
        num_tags = ratio
        num_user_ratings = ratio * 200
        print("Start Loading")
        # Создание пользователей и профилей
        with transaction.atomic():
            existing_usernames = set(User.objects.values_list('username', flat=True))
            user_list = [User(username=f'user_{i}') for i in range(num_users) if f'user_{i}' not in existing_usernames]
            User.objects.bulk_create(user_list)

            profile_list = [Profile(user=user) for user in user_list]
            Profile.objects.bulk_create(profile_list)
            print("Profile created\n")

        # Создание тегов
        with transaction.atomic():
            tag_list = [Tag(name=f'Tag {i}') for i in range(num_tags)]
            Tag.objects.bulk_create(tag_list)
            print("Tags created\n")

        # Создание вопросов
        all_tags = list(Tag.objects.all())
        all_profiles = list(Profile.objects.all())

        with transaction.atomic():
            question_list = []
            for i in range(num_questions):
                question_list.append(Question(
                    title=f'Question Title {i}',
                    text=f'Question text {i}',
                    rating=random.randint(1, 100),
                    profile=random.choice(all_profiles)
                ))
                k_ = random.choice(range(1, 20))
                question_list[i].tags.add(random.choices(all_tags, k=k_))
            Question.objects.bulk_create(question_list)
            print("Questions created\n")

        # Создание ответов
        all_questions = list(Question.objects.all())

        with transaction.atomic():
            answer_list = [
                Answer(
                    text=f'Answer text {i}',
                    correct=random.choice([True, False]),
                    profile=random.choice(all_profiles),
                    question=random.choice(all_questions)
                ) for i in range(num_answers)
            ]
            Answer.objects.bulk_create(answer_list)
            print("Answers created\n")
        question_like_list = []
        answer_like_list = []
        len_profile = len(all_profiles)
        # Создание оценок для вопросов и ответов
        with transaction.atomic():
            order_100 = random.choice(range(len_profile // 90))
            order_1 = random.choice(range(100))
            for i in range(len_profile):
                for j in range(100 * order_100 + order_1, 100 * (order_100 + 1) + order_1):
                    question_like_list.append(QuestionLike(profile=all_profiles[i], question=all_questions[j],
                                                           status=random.choice([True, False])))
                    answer_like_list.append(AnswerLike(profile=all_profiles[i], answer=answer_list[j],
                                                       status=random.choice([True, False])))
                if (i % 100) == 0 and i != 0:
                    print(str(i // len_profile) + "% creating loading likes")
            QuestionLike.objects.bulk_create(question_like_list)
            AnswerLike.objects.bulk_create(answer_like_list)
