from django.shortcuts import render
from django.http import HttpResponse
# Create your views here.

def home(request):
    return render(request, 'Manager/dashboard.html')

def my_sheets(request):
    return render(request, 'Manager/my_sheets.html')

def my_spells(request):
    return render(request, 'Manager/my_spells.html')

def my_General_abilities(request):
    return render(request, 'Manager/GeneralAbilities.html')

def Class_abilities(request):   
    return render(request, 'Manager/ClassAbilities.html')

def Race_abilities(request):
    return  render(request, 'Manager/RaceAbilities.html')