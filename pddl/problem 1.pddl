(define (problem Pr1)
    (:domain Gym)
    (:objects
        hall  home gym - room
        Alex Ivan Vova - person
        Olga - trainer
        o1 - outside
        i -  inside
        h - hall
        
    )
    
    (:init
        (in o1 home)
        (in h hall)
        (in i gym)
        (at Ivan o1)
        (at Alex o1)
        (at Vova  o1)
        (at  Olga o1)
        (hasSportsSuit Ivan)
        (hasSportsSuit Alex)
        (hasSportsSuit Vova )
        (not (hasSportsSuit Olga))
        (hasPass Vova)
        (hasPass Alex)
        (hasPass Olga)
        (hasPass Ivan)
        (isClothesOn Ivan)
        (isClothesOn Alex)
        (isClothesOn Vova )
        
        
        
       
        
        
    )
    
    (:goal
        (and (and (at  Alex i) (EnteredTheGym Alex)) (and (at Ivan i) (EnteredTheGym Ivan)) (and (at Vova i) (EnteredTheGym Vova)) (and (at Olga h)))
    )

)