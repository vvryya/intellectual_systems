(
    define (domain Gym)
    (:requirements :strips :adl)
    (:types
        object place - type
        person trainer - man
        inside outside hall - location
        room location - place
        
    )
    
    (:predicates
        (at ?x - object ?loc - location)
        (in ?loc - location ?r - room)
        (SportsSuitOn ?p - person)
        (hasPass ?m -  man)
        (isClothesOn ?p - person )
        (hasSportsSuit ?p - person )
        (CanGetIn ?m -  man)
        (isInBuilding ?m -  man)
        (isInHall ?m -  man)
        (EnteredTheGym ?p - person )
    )
    
    (:action ComeFromSomewhere 
        :parameters(?x -  outside ?y -  outside ?r - room ?p -  person)
        :precondition(and (at ?p ?x) (not (= ?x ?y)) (in ?x ?r) (in ?y ?r))
        :effect(and (not (at ?p ?x)) (at ?p ?y))
    )
     (:action EnterTheBuilding
        :parameters(?x -  outside ?h - hall ?m - man)
        :precondition(and (at ?m ?x) (not(CanGetIn ?m)))
        :effect(and (not (at ?m ?x)) (at ?m ?h) (isInHall ?m))
    )
    
    (:action PassForAPerson
        :parameters(?p -  person)
        :precondition(and (SportsSuitOn ?p) (hasPass ?p)(isInHall ?p))
        :effect(CanGetIn ?p)
    )
    
    (:action PassForATrainer 
        :parameters(?t - trainer)
        :precondition(and (hasPass ?t) (isInHall ?t))
        :effect(CanGetIn ?t)
    )
        (:action EnterTheHall
        :parameters(?h - hall ?y -  inside ?m - man)
        :precondition(and (at ?m ?h) (CanGetIn ?m))
        :effect(and (not (at ?m ?h)) (at ?m ?y) (isInBuilding ?m))
    )
    
   
    

    
 (:action Undress
        :parameters(?p -  person)
        :precondition(isClothesOn ?p)
        :effect(not(isClothesOn ?p))
    )
    (:action PutOnSuit
        :parameters(?p -  person)
        :precondition(and (not(SportsSuitOn ?p)) (hasSportsSuit ?p))
        :effect(SportsSuitOn ?p)
    )
    
    
    (:action EnterTheGym
        :parameters(?p - person)
        :precondition(and (not(isClothesOn ?p)) (not(EnteredTheGym ?p)) )
        :effect(EnteredTheGym ?p)
    )
    
   
    
)