(define (problem P2) (:domain  Gym)
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
        (at Vova o1)
        (at Alex o2)
        (at Ivan o2)
        (at Olga o1)
        (hasSportsSuit Ivan)
        (hasSportsSuit Alex)
        (hasSportsSuit Vova) 
        (hasPass Ivan)
        (hasPass Alex)
        (hasPass Vova)
        (hasPass Olga)
        (isClothesOn Alex)
        (isClothesOn Ivan)
        (isClothesOn Vova)
        
        
       
        
        
    )
    
    (:goal
        (and (and (at Alex  i)(EnteredTheGym Alex ))  (and (at Vova o1)) (and (at Ivan i) (EnteredTheGym Ivan)) (and (at Olga h)))
    )

)