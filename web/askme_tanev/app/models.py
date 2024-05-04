from django.contrib.auth.models import User
from django.db.models import Count
from django.db import models

from django.db.models import Sum, Case, When


class ProfileManager(models.Manager):
    def by_rating(self):
        return self.annotate(
            total_likes=Sum(
                Case(
                    When(question__questionlike__status=True, then=1),
                    When(question__questionlike__status=False, then=0),
                    default=0,
                    output_field=models.IntegerField()
                )
            )
        ).order_by('-total_likes')


class Profile(models.Model):
    user = models.OneToOneField(User, on_delete=models.CASCADE)
    avatar = models.ImageField(upload_to='profile_pics', null=True, blank=True)

    objects = ProfileManager()


class QuestionManager(models.Manager):
    def by_rating(self):
        return self.order_by('-rating')

    def by_creation_date(self):
        return self.order_by('-created_at')

    def for_tag(self, tag_id):
        return self.filter(tags__id=tag_id)


class AnswerManager(models.Manager):
    def by_creation_date(self, question_id):
        return self.filter(question_id=question_id).order_by('-created_at')


class TagManager(models.Manager):
    def by_popular(self):
        return self.annotate(num_questions=Count('question')).order_by('-num_questions')


class Tag(models.Model):
    name = models.CharField(max_length=100)

    objects = TagManager()

    def get_url(self):
        return "\\tag\\" + str(self.id)

    def __str__(self):
        return self.name


class Question(models.Model):
    title = models.CharField(max_length=100)
    text = models.TextField()
    rating = models.IntegerField(default=0)
    created_at = models.DateTimeField(auto_now_add=True)
    profile = models.ForeignKey(Profile, on_delete=models.CASCADE)
    tags = models.ManyToManyField(Tag, blank=True)

    objects = QuestionManager()


class Answer(models.Model):
    text = models.TextField()
    correct = models.BooleanField()
    created_at = models.DateTimeField(auto_now_add=True)
    rating = models.IntegerField(default=0)
    profile = models.ForeignKey(Profile, on_delete=models.CASCADE)
    question = models.ForeignKey(Question, on_delete=models.CASCADE)

    objects = AnswerManager()


class AnswerLike(models.Model):
    status = models.BooleanField(default=True)
    answer = models.ForeignKey(Answer, on_delete=models.CASCADE)
    profile = models.ForeignKey(Profile, on_delete=models.CASCADE)

    class Meta:
        unique_together = ('profile', 'answer')


class QuestionLike(models.Model):
    status = models.BooleanField(default=True)
    question = models.ForeignKey(Question, on_delete=models.CASCADE)
    profile = models.ForeignKey(Profile, on_delete=models.CASCADE)

    class Meta:
        unique_together = ('profile', 'question')
