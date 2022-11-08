(define (problem Pp3)
    (:domain Gym)
    
    (:objects
        hall onWay home gym - room
        Alex Ivan Vova - person
        Olga - trainer
        o1 o2  - outside
        i -  inside
        h - hall
    )
    
    (:init
        (in o1 home)
        (in o2 onWay)
        (in h hall)
        (in i gym)
        (at Alex o1)
        (at Ivan h)
        (at Vova h)
        (at Olga h)
        (hasSportsSuit Ivan)
        (hasSportsSuit Vova)
        (hasSportsSuit Alex)
        (not (hasSportsSuit Olga))
        (hasPass Ivan)
        (hasPass Vova)
        (hasPass Alex)   
        (hasPass Olga)
        (isClothesOn Alex)
        (not(isClothesOn Ivan))
        (not(isClothesOn Vova))
        (isInHall Vova)
        (isInHall Ivan)
        (isInHall Olga)
        
        
        
    
        
    )
    
    (:goal
        (and (and (at Ivan i) (EnteredTheGym Ivan)) (and (at Alex i) (EnteredTheGym Alex)) (and (at Vova i) (EnteredTheGym Vova))  (and (at Olga h)))
    )

)