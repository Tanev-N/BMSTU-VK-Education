import random
from django.contrib.auth.models import User
from django.core.management import BaseCommand
from django.db import transaction
from app.models import Profile, Tag, Question, Answer, QuestionLike, AnswerLike
from faker import Faker
import os
from django.core.files import File

from askme_tanev.settings import MEDIA_ROOT


class Command(BaseCommand):

    def add_arguments(self, parser):
        parser.add_argument('ratio', type=int)

    def handle(self, *args, **options):
        fake = Faker()
        ratio = options['ratio']
        num_users = ratio
        num_questions = ratio * 10
        num_answers = ratio * 100
        num_tags = ratio
        print("Start Loading")
        # Создание пользователей и профилей
        with transaction.atomic():
            user_list = [User(username=fake.unique.name(), email=fake.unique.email(), password=fake.unique.text()[:10])
                         for i in range(num_users)]
            User.objects.bulk_create(user_list)
            avatars_path = MEDIA_ROOT / 'profile_pics/avatars_for_generating'
            image_files = [os.path.join(avatars_path, f) for f in os.listdir(avatars_path) if os.path.isfile(os.path.join(avatars_path, f))]
            profile_list = []
            files_to_close = []
            for user in user_list:
                random_image_path = random.choice(image_files)
                image_file = open(random_image_path, 'rb')
                files_to_close.append(image_file)
                avatar_file = File(image_file, name=os.path.basename(random_image_path))
                profile = Profile(user=user, avatar=avatar_file)
                profile_list.append(profile)
            Profile.objects.bulk_create(profile_list)
            print("Profile created\n")
            for file in files_to_close:
                file.close()

        # Создание тегов
        with transaction.atomic():
            tag_list = [Tag(name=fake.unique.text()[:10]) for i in range(num_tags)]
            Tag.objects.bulk_create(tag_list)
            print("Tags created\n")

        # Создание вопросов
        all_tags = Tag.objects.all()
        all_profiles = Profile.objects.all()

        with transaction.atomic():
            question_list = []
            for i in range(num_questions):
                question_list.append(Question(
                    title=fake.text()[:20],
                    text=fake.text(),
                    rating=random.randint(1, 100000),
                    profile=random.choice(all_profiles)
                ))

            Question.objects.bulk_create(question_list)
            print("Questions created\n")

        # Add relation ManyToMany
        with transaction.atomic():
            for i in range(num_questions):
                k_ = random.choice(range(1, 20))
                tagsss = random.choices(all_tags, k=k_)
                for j in range(k_):
                    question_list[i].tags.add(tagsss[j])
        # Создание ответов
        all_questions = Question.objects.all()
        all_profiles = Profile.objects.all()

        print("tags relation created\n")

        with transaction.atomic():
            answer_list = [
                Answer(
                    text=fake.text()[:50],
                    correct=random.choice([True, False]),
                    rating=random.randint(1, 500),
                    profile=random.choice(all_profiles),
                    question=random.choice(all_questions),
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
                    print(str(i / len_profile*100) + "% creating loading likes")
            QuestionLike.objects.bulk_create(question_like_list)
            AnswerLike.objects.bulk_create(answer_like_list)
